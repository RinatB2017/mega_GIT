#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

#include <connection.h>

#include "buslistmodel.h"
#include "bus.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    iot::Connection::instance()->setUserName("haor");
    iot::Connection::instance()->setPassword("KaarloKettu2015");

    QQmlApplicationEngine engine;
    qmlRegisterType<BusListModel>("com.wapice.models", 1, 0, "BusListModel");
    qmlRegisterType<Bus>("com.wapice.models", 1, 0, "Bus");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
