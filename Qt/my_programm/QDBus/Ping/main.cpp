#include <stdio.h>

#include <QObject>
#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>
#include <QDebug>

#include "Ping.h"
#include "../serviceNameAndProperty.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Ping ping;

    if (!QDBusConnection::sessionBus().isConnected()) 
    {
             fprintf(stderr, "Cannot connect to the D-Bus session bus.\n"
                     "To start it, run:\n"
                     "\teval `dbus-launch --auto-syntax`\n");
             return 1;
    }
    qDebug()<<"Ping connected to D-bus";


    QDBusConnectionInterface *iface = QDBusConnection::sessionBus().interface();
    QDBusServiceWatcher watcher;
    watcher.setConnection(QDBusConnection::sessionBus());;
    watcher.addWatchedService(ping.m_aviableServiceName);
    //    QObject::connect(&watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)),&ping,  SLOT(manageConnection(QString,QString,QString)));
    QObject::connect(&watcher, SIGNAL(serviceRegistered(QString)), &ping, SLOT(connectToService(QString)));

    QStringList registedServices = iface->registeredServiceNames();
    if(registedServices.contains(ping.m_aviableServiceName))
        ping.connectToService(ping.m_aviableServiceName);
    return a.exec();
}
