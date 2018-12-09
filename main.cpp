#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include "RPiTank.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
	QStringList args = app.arguments();
	int port = 8880;
	if (args.contains("-p")) {
		bool ok;
		int tmpport = args.at(args.indexOf("-p") + 1).toInt(&ok);
		if (ok) {
			port = tmpport;
		}
	}
    RPiTank rpitank(port);
    return app.exec();
}
