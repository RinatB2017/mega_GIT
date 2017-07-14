//--------------------------------------------------------------------------------
#include <QNetworkInterface>
#include <QHostAddress>
#include <QUdpSocket>
//--------------------------------------------------------------------------------
#include "udp_server.hpp"
//--------------------------------------------------------------------------------
UDP_Server::UDP_Server(QWidget *parent) :
    QWidget(parent),
    udpSocket(0)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),  parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent, SIGNAL(error(QString)));
    }

    init();
}
//--------------------------------------------------------------------------------
UDP_Server::~UDP_Server()
{
    delete udpSocket;
}
//--------------------------------------------------------------------------------
QString UDP_Server::getLocalAddress(void)
{
    QString ipAddress;

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    return ipAddress;
}
//--------------------------------------------------------------------------------
void UDP_Server::init()
{

}
//--------------------------------------------------------------------------------
int UDP_Server::createServerOnPort(const QHostAddress address, unsigned int port)
{
    emit info(QString("Создание сервера %1:%2")
              .arg(address.toString())
              .arg(port));
    udpSocket = new QUdpSocket(this);
    if(!udpSocket->bind(address, port))
    {
        emit error("Ошибка: "+udpSocket->errorString());
        return -1;
    }
    emit info("Сервер создан");

    emit debug(QString("IP: %1").arg(address.toString()));
    emit debug(QString("Port: %1").arg(port));

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    return 0;
}
//--------------------------------------------------------------------------------
void UDP_Server::readPendingDatagrams(void)
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        emit debug(datagram.data());
        emit send(datagram);
    }
}
//--------------------------------------------------------------------------------
