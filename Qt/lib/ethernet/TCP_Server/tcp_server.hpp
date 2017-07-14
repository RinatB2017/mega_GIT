//--------------------------------------------------------------------------------
#ifndef TCP_SERVER_H
#define TCP_SERVER_H
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QByteArray>
#include <QWidget>
#include <QString>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Processor;
class QTcpServer;
class QTcpSocket;
class QWidget;
//--------------------------------------------------------------------------------
class TCP_Server : public MyWidget
{
    Q_OBJECT
public:
    explicit TCP_Server(QWidget *parent = 0);
    ~TCP_Server();

signals:
    void output(const QByteArray &);

public slots:    
    bool createServerOnPort(const QHostAddress address, unsigned int);
    void closeServer(void);
    void input(const QByteArray &data);

private slots:
    void newConnect(void);
    void clientReadyRead(void);
    void clientDisconnected(void);

private:
    Processor *processor = 0;
    QTcpServer *tcpServer = 0;
    QTcpSocket *clientConnection = 0;

};
//--------------------------------------------------------------------------------
#endif
