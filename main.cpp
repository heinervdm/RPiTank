#include <QCoreApplication>
#include "RPiTank.h"


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    RPiTank rpitank;
    return app.exec();
}
