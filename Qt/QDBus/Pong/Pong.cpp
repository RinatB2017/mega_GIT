#include "Pong.h"
#include <QDebug>
#include <QDBusMetaType>
#include <QDBusConnection>
#include <QDBusMessage>

PongAdapter::PongAdapter(Pong *parent) :
    QDBusAbstractAdaptor(parent)
{
    m_parentPong = parent;
    qRegisterMetaType<TestStructure>("TestStructure");
    qDBusRegisterMetaType<TestStructure>();

}

QString PongAdapter::imageDataSharedId()
{
    return m_parentPong->imageDataSharedId();
}

TestStructure PongAdapter::structureField()
{
    return m_parentPong->structureField();
}

QString Pong::imageDataSharedId()
{
    return m_imageDataSharedId;
}

TestStructure &Pong::structureField()
{
    qDebug()<<"Me calld"<<QDBusConnection::sessionBus().baseService()<<message().service();
    emit callingMe(QString("Panic"), QString("Super panic"));
    test.str = QString("ku");
    test.id =2;
    return test;
}
