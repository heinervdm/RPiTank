#include "RPiTank.h"

#include <QTimer>
#include <iostream>
#include <QTcpSocket>
#include <QThread>

RPiTank::RPiTank(int port)
{
	httpThread = new QThread(this);
	http = new HttpProvider(port);
	http->moveToThread(httpThread);
	connect(http, SIGNAL(destroyed()), httpThread, SLOT(deleteLater()));
	connect(httpThread, SIGNAL(started()), http, SLOT(init()));
	httpThread->start();

	gpioThread = new QThread(this);
	gpio = new GPIO();
	gpio->moveToThread(gpioThread);
	connect(http, SIGNAL(newData(int, int)), gpio, SLOT(set(int,int)), Qt::QueuedConnection);
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
