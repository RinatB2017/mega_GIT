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

    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    return ipAddress;
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

    connect(udpSocket,  &QUdpSocket::readyRead, this,   &UDP_Client::readPendingDatagrams);
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
void UDP_Client::setAddress(const QHostAddress &address)
{
#ifdef DEBUG
    emit debug(QString("set address %1").arg(address.toString()));
#endif
    this->address = address.toString();
}
//--------------------------------------------------------------------------------
void UDP_Client::setPort(quint16 port)
{
#ifdef DEBUG
    emit debug(QString("set port %1").arg(port));
#endif
    this->port = port;
}
//--------------------------------------------------------------------------------
QByteArray UDP_Client::input(const QByteArray &data)
{
    return send_data(data);
}
//--------------------------------------------------------------------------------
QByteArray UDP_Client::send_data(const QByteArray &data)
{
    QByteArray tmp;

    tmp.clear();
    emit info(QString("connect to host %1:%2").arg(address).arg(port));
    udpSocket->connectToHost(address,
                             static_cast<quint16>(port));
    if (!udpSocket->waitForConnected(3000))
    {
        emit error(tr("Сервер не отвечает!"));
        return nullptr;
    }
    udpSocket->write(data);
    if (udpSocket->waitForBytesWritten(3000))
    {
        emit info(tr("Данные переданы!"));
    }
    else
    {
        emit error(tr("Данные передать не удалось!"));
    }

    if(udpSocket->waitForReadyRead (5000))
    {
        tmp = udpSocket->readAll();
        emit info(tr("Данные получены!"));
    }
    else
    {
        emit error(tr("Данные получить не удалось!"));
    }

    udpSocket->disconnectFromHost();
    return tmp;
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
