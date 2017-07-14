#include "Ping.h"
#include "../serviceNameAndProperty.h"
#include <QDBusConnectionInterface>
#include <QDebug>
#include <QDBusMetaType>

const QString Ping::_propertyName(QUIOTING(IMAGE_DATA_SHARED_ID));
Ping::Ping(QObject *parent) :
    QObject(parent)
{
    m_interface = NULL;
    m_interfaceName = QString(BUFFER_NAME);
    m_aviableServiceName = QString(SERVICE_NAME);
    qRegisterMetaType<TestStructure>("TestStructure");
    qDBusRegisterMetaType<TestStructure>();
}

void Ping::manageConnection(const QString& name, const QString &oldVAlue, const QString &newValue)
{
    Q_UNUSED(oldVAlue)
    if(name != m_aviableServiceName)
        return;
    if(newValue.isEmpty())
        disconnect(name);
    else
        connectToService(name);

}

void Ping::connectToService(const QString &name)
{
    if(name != m_aviableServiceName)
        return;
    qDebug()<<"Connceting";
    m_interface = new QDBusInterface(name, "/", m_interfaceName, QDBusConnection::sessionBus(), this);
    QObject::connect(m_interface, SIGNAL(callingMe(QString, QString)), this, SLOT(reacoOnMeCalling(QString, QString)));
    if(!m_interface->isValid()){
        qDebug()<<"Invalid interface"<<m_interface->lastError();
        delete m_interface;
        m_interface = NULL;
        return;
    }
    qDebug()<<m_interface->interface();

    QVariant var("sss");
    var = m_interface->property("imageDataSharedId");
    qDebug()<<var;
    QDBusReply<TestStructure> reply= m_interface->call("structureField");
    if(reply.isValid())
    {
        TestStructure testStructure = reply.value();
        qDebug()<<testStructure.id<<testStructure.str;
    }
}

void Ping::disconnect(const QString &name)
{
    if(name != m_aviableServiceName)
        return;
    if(name != m_interface->service())
        return;

    delete m_interface;
    m_interface = NULL;
    qDebug()<<"Disconnect";
}

void Ping::reacoOnMeCalling(QString message, QString message2)
{
    qDebug()<<message<<message2;
}
