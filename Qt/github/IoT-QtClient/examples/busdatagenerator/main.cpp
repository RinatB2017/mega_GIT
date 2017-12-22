#include <QCoreApplication>

#include <connection.h>

#include "buscreator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    iot::Connection::instance()->setUserName("haor");
    iot::Connection::instance()->setPassword("KaarloKettu2015");

    BusCreator busCreator(3);
    busCreator.create();

    return a.exec();
}
