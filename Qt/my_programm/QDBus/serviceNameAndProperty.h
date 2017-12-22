#pragma once
#include<QMetaType>
#include <QString>
#include <QDBusArgument>
#define SERVICE_NAME "ru.sonarh.dbus.pong"
#define BUFFER_NAME "ru.buffer"
#define IMAGE_DATA_SHARED_ID imageDataSharedId
#define QUIOTING(text) #text


struct TestStructure{
    int id;
    QString str;
};
Q_DECLARE_METATYPE(TestStructure)

static QDBusArgument& operator <<(QDBusArgument &argument, const TestStructure & arg)
{
    argument.beginStructure();
    argument<<arg.id<<arg.str;
    argument.endStructure();
    return argument;
}

static const QDBusArgument& operator >>(const QDBusArgument &argument, TestStructure & arg)
{
    argument.beginStructure();
    argument>>arg.id>>arg.str;
    argument.endStructure();
    return argument;
}
