//--------------------------------------------------------------------------------
#ifndef _TEST_THREAD_H
#define _TEST_THREAD_H
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QtNetwork>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QtDebug>
#endif
//--------------------------------------------------------------------------------
#include "proto.hpp"
//--------------------------------------------------------------------------------
class InfoEvent : public QEvent
{
public:
    InfoEvent(const QString& s, int t = 0, bool b = false) : QEvent(QEvent::User)
    {
        type = t;
        text = s;
        bold = b;
        srl = false;
    }
    int type;
    QString text;
    bool bold;
    bool srl;
};
//--------------------------------------------------------------------------------
class SerialInfoEvent : public InfoEvent
{
public:
    SerialInfoEvent(const QString& s) : InfoEvent(s)
    {
        srl = true;
    }
};
//--------------------------------------------------------------------------------
class QTcpSocket;
//--------------------------------------------------------------------------------
class TestThread : public QThread
{
    Q_OBJECT
    
public:
    TestThread(QObject *parent);
    void stop();
    void setSleepInterval(int);
    void setConnectSettings(const QHostAddress&, int);
    void setSerialDescriptor(int);

protected:
    void print(const QString&, int = 0, bool = false);
    void print_serial(const QString&);
    bool init_proto(const QString addr, unsigned int port);
    void delete_proto();
    void connectToHost();
    QString test_name;
    QObject* receiver;
    bool stopped;
    QMutex mutex;
    int sleep_interval;
    Proto* proto;
    QTcpSocket* socket;
    QHostAddress address;
    int port, descriptor;

private slots:
    // --- socket slots
    void OnSocketConnected();
    void OnSocketDisconnected();
    void OnSocketError(QAbstractSocket::SocketError);
    void OnStateChanged(QAbstractSocket::SocketState);
    
private:
    QString stateString(QAbstractSocket::SocketState state);
};
//--------------------------------------------------------------------------------
#endif
