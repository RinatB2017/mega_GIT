#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusError>
#include <QDebug>

#include "Pong.h"
#include "../serviceNameAndProperty.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QDBusConnection connection = QDBusConnection::sessionBus();
    Pong pong;
    if( ! connection.registerObject("/", &pong))
    {
        fprintf(stderr, "%s\n",
                qPrintable("Can't register object"));
        exit(1);
    }
    qDebug()<<"Pong connected to D-bus";

    if (!connection.registerService(SERVICE_NAME)) 
    {
        fprintf(stderr, "%s\n",
                qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }
    qDebug()<<"Test service start";
    return a.exec();
}
