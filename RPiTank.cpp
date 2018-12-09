#include "RPiTank.h"

#include <QTimer>
#include <iostream>
#include <QTcpSocket>
#include <QThread>

RPiTank::RPiTank()
{
	httpThread = new QThread(this);
	http = new HttpProvider(8080);
	http->moveToThread(httpThread);
	connect(http, SIGNAL(destroyed()), httpThread, SLOT(deleteLater()));
	httpThread->start();

	gpioThread = new QThread(this);
	gpio = new GPIO();
	gpio->moveToThread(gpioThread);
	connect(http, SIGNAL(newData(int, int)), gpio, SLOT(onDataReceived(int,int)), Qt::QueuedConnection);
	connect(gpio, SIGNAL(destroyed()), gpioThread, SLOT(deleteLater()));
	gpioThread->start();
}

RPiTank::~RPiTank()
{
	if (http) {
		http->deleteLater();
		http = nullptr;
	}
	if (gpio) {
		gpio->deleteLater();
		gpio = nullptr;
	}
}
