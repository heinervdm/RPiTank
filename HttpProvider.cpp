/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  Thomas Zimmermann <zimmermann@vdm-design.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "HttpProvider.h"
#include <QTcpSocket>
#include <QDateTime>
#include <QByteArray>
#include <QFile>
#include <QDirIterator>

HttpProvider::HttpProvider(int port) : httpPort(port){

}

HttpProvider::~HttpProvider() {
	if (tcpServer) {
		tcpServer->deleteLater();
		tcpServer = nullptr;
	}
}

void HttpProvider::init() {
	if (!tcpServer) {
		tcpServer = new QTcpServer();
		connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
		tcpServer->listen(QHostAddress::Any, httpPort);
	}
	if (!timeoutTimer) {
		timeoutTimer = new QTimer();
		connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
		timeoutTimer->setInterval(1000);
		timeoutTimer->start();
	}
}

void HttpProvider::onNewConnection() {
	if (!tcpServer) {
		return;
	}
	QTcpSocket *socket = tcpServer->nextPendingConnection();
	while (socket) {
		if (timeoutTimer) {
			timeoutTimer->start();
		}
		bool header = true;
		QByteArray path;
		QByteArray host;
		bool post = false;
		connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
		quint64 time = QDateTime::currentMSecsSinceEpoch();
		QByteArray buffer;
		socket->waitForReadyRead(1000);
		buffer = socket->readAll();
		while (buffer.contains('\n')) {
			int index = buffer.indexOf('\n');
			QByteArray data = buffer.left(index);
			buffer.remove(0, index);
			if (data.startsWith("GET")) {
				QList<QByteArray> l = data.split(' ');
				if (l.size() > 1) {
					path = l.at(1);
					handleGet(socket, QString::fromLatin1(path));
				}
				break;
			} else if (data.startsWith("POST")) {
				post = true;
				QList<QByteArray> l = data.split(' ');
				if (l.size() > 1) {
					path = l.at(1);
				}
			} else if (data.startsWith("Host:")) {
				host = data.mid(6);
			} else if (!header && post) {
				handlePost(socket, QString::fromLatin1(path), QString::fromLatin1(data));
				break;
			} else if (data.isEmpty()) {
				header = false;
			}
		}
		socket->close();
		socket->deleteLater();
		socket = tcpServer->nextPendingConnection();
	}
}

void HttpProvider::handleGet(QTcpSocket * socket, QString path) {
	// HTTP/1.1 200 OK
	// Date: Sun, 02 Dec 2018 14:15:08 GMT
	// Server: Apache
	// Vary: Accept-Encoding,Host,User-Agent
	// Last-Modified: Thu, 29 Nov 2018 06:40:21 GMT
	// ETag: "2ca0-57bc7f4eb3a64"
	// Accept-Ranges: bytes
	// Content-Length: 11424
	// Cache-Control: max-age=0
	// Expires: Sun, 02 Dec 2018 14:15:08 GMT
	// Content-Encoding: gzip
	// Keep-Alive: timeout=3, max=100
	// Connection: Keep-Alive
	// Content-Type: text/html
	if (path == "/") {
		path = "/index.html";
	} else if (path.startsWith("/go?")) {
		path = path.remove(0,4).trimmed();
		int left = 0;
		int right = 0;
		QStringList parts = path.split('&');
		for (QString p : parts) {
			QStringList var = p.split('=');
			if (var.at(0) == "l") {
				left = var.at(1).toInt();
			} else if (var.at(0) == "r") {
				right = var.at(1).toInt();
			}
		}

		emit newData(left, right);

		socket->write(QString("HTTP/1.1 204 No Content\nContent-Length: 0\nContent-Type: text/html; charset=iso-8859-1\nConnection: Closed\n\n").toLatin1());
		socket->waitForBytesWritten(1000);

		return;
	}

	QFile f(QString(":/web%1").arg(path));
	if (f.open(QIODevice::ReadOnly | QFile::Text)) {
		QTextStream in(&f);
		QString data = in.readAll();
		if (data.contains("localhost") && !socket->localAddress().isNull()) {
			bool conversionOK = false;
			QHostAddress ip4Address(socket->localAddress().toIPv4Address(&conversionOK));
			QString ipString;
			if (conversionOK) {
				ipString = ip4Address.toString();
			} else {
				ipString = socket->localAddress().toString();
			}
			data.replace("localhost",ipString);
		}
		socket->write(QString("HTTP/1.1 200 OK\nContent-Length: %1\nContent-Type: %2\nConnection: Closed\n\n%4\n").arg(QString::number(data.size()+1)).arg("text/html; charset=iso-8859-1").arg(data).toLatin1());
		socket->waitForBytesWritten(1000);
	} else {
		// send Error 404
		socket->write(QString("HTTP/1.1 404 NOT FOUND\nContent-Length: 0\nContent-Type: text/html; charset=iso-8859-1\nConnection: Closed\n\n").toLatin1());
		socket->waitForBytesWritten(1000);
	}
}

void HttpProvider::handlePost(QTcpSocket * socket, QString path, QString data) {
	socket->write(QString("HTTP/1.1 204 No Content\nContent-Length: 0\nContent-Type: text/html; charset=iso-8859-1\nConnection: Closed\n\n").toLatin1());
	socket->waitForBytesWritten(1000);

	int left = 0;
	int right = 0;
	QStringList parts = data.split('&');
	for (QString p : parts) {
		QStringList var = p.split('=');
		if (var.at(0) == "l") {
			left = var.at(1).toInt();
		} else if (var.at(0) == "r") {
			right = var.at(1).toInt();
		}
	}
	emit newData(left, right);
}

void HttpProvider::onTimeout() {
	emit newData(0, 0);
}
