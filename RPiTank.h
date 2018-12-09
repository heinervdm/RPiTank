#ifndef RPiTank_H
#define RPiTank_H

#include <QObject>
#include <QThread>

#include "HttpProvider.h"
#include "GPIO.h"

class RPiTank : public QObject
{
	Q_OBJECT

public:
	RPiTank();
	virtual ~RPiTank();

private:
	HttpProvider *http;
	QThread *httpThread;

	GPIO *gpio;
	QThread *gpioThread;
};

#endif // RPiTank_H
