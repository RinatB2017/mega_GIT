//--------------------------------------------------------------------------------
#include <QUdpSocket>
//--------------------------------------------------------------------------------
#include <QNetworkInterface>
#include <QHostAddress>
#include <QFileDialog>
#include <QList>
//--------------------------------------------------------------------------------
#include "udp_client.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
UDP_Client::UDP_Client(QWidget *parent) :
    MyWidget(parent),
    parent(parent)
{
    init();
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
    bool res = false;

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
void UDP_Client::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool UDP_Client::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void UDP_Client::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void UDP_Client::save_setting(void)
{

}
//--------------------------------------------------------------------------------
