#ifndef PING_H
#define PING_H

#include <QObject>
#include <QDBusAbstractInterface>
#include <qdbusinterface.h>

class Ping : public QObject
{
    Q_OBJECT
public:
    explicit Ping(QObject *parent = 0);
public slots:
    void manageConnection(const QString &name, const QString &oldVAlue, const QString &newValue);
    void connectToService(const QString &name);
    void disconnect(const QString &name);
    void reacoOnMeCalling(QString message, QString message2);
public:
    QString m_aviableServiceName;
private:
    QDBusInterface *m_interface;
    QString m_interfaceName;
    static const QString _propertyName;
};

#endif // PING_H
