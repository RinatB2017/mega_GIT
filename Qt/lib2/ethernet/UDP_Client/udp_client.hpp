//--------------------------------------------------------------------------------
#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H
//--------------------------------------------------------------------------------
#include <QWidget>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QByteArray;
class QString;
class QUdpSocket;
//--------------------------------------------------------------------------------
class UDP_Client : public MyWidget
{
    Q_OBJECT

public:
    UDP_Client(QWidget *parent = 0);
    ~UDP_Client();

public slots:
    void send(const QString &data);

private slots:
    void readPendingDatagrams(void);

private:
    QWidget *parent;
    QUdpSocket *udpSocket;
    QString fileName;
    QString address;
    unsigned int port;

    void send_data(QByteArray);
    QString getLocalAddress(void);
    void init(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
