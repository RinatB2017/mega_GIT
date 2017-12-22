#ifndef PONG_H
#define PONG_H
#include <QDBusAbstractAdaptor>
#include <QDBusVariant>
#include <QDBusArgument>
#include <QDBusContext>

#include "../serviceNameAndProperty.h"

class Pong;

class PongAdapter : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", BUFFER_NAME)
    Q_PROPERTY(QString IMAGE_DATA_SHARED_ID READ imageDataSharedId)
public:
    explicit PongAdapter(Pong *parent);

    QString imageDataSharedId();

public slots:
    TestStructure structureField();
signals:
    void callingMe(QString, QString);
private:
    Pong * m_parentPong;

};

class Pong : public QObject, public QDBusContext
{
    Q_OBJECT
public:
    Pong()
    {
        m_pongAdapter = new PongAdapter(this);
        QObject::connect(this, SIGNAL(callingMe(QString,QString)), m_pongAdapter, SIGNAL(callingMe(QString, QString)));
        m_imageDataSharedId = "testImageBufferId";

    }
public:
    QString imageDataSharedId();
    TestStructure& structureField();

signals:
    void callingMe(QString, QString);
private:
    PongAdapter *m_pongAdapter;
    QString m_imageDataSharedId;
    TestStructure test;

};
#endif // PONG_H>
