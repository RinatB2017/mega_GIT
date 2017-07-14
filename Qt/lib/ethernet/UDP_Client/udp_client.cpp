//--------------------------------------------------------------------------------
#include <QUdpSocket>
//--------------------------------------------------------------------------------
#include <QNetworkInterface>
#include <QHostAddress>
#include <QFileDialog>
#include <QDebug>
#include <QList>
//--------------------------------------------------------------------------------
#include "udp_client.hpp"
//--------------------------------------------------------------------------------
UDP_Client::UDP_Client(QWidget *parent) :
    QWidget(parent),
    parent(parent),
    udpSocket(0)
{
    init();
}
//--------------------------------------------------------------------------------
void UDP_Client::connect_log(void)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void UDP_Client::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
QString UDP_Client::getLocalAddress(void)
{
    QString ipAddress;

#if 0
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
#endif
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    return ipAddress;
}
//--------------------------------------------------------------------------------
void UDP_Client::send_data(QByteArray data)
{
    qint64 res;

    res = udpSocket->writeDatagram(data, QHostAddress(address), port);
    if(res == -1)
    {
        emit info(udpSocket->errorString());
    }
    else
    {
        emit info(tr("Данные переданы успешно"));
    }
}
//--------------------------------------------------------------------------------
void UDP_Client::init()
{
    bool res;

    connect_log();

    address = "127.0.0.1";
    port = 10000;
    emit info(QString("address %1 port %2").arg(address).arg(port));

    udpSocket = new QUdpSocket();
    res = udpSocket->bind(QHostAddress(address), port);
    emit info(res ? "bind is true" : "bind is false");

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}
//--------------------------------------------------------------------------------
void UDP_Client::readPendingDatagrams(void)
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    }
}
//--------------------------------------------------------------------------------
UDP_Client::~UDP_Client()
{
}
//--------------------------------------------------------------------------------
void UDP_Client::send(const QString &data)
{
    QByteArray ba;

    ba.clear();
    ba.append(data);
    send_data(ba);
}
//--------------------------------------------------------------------------------
