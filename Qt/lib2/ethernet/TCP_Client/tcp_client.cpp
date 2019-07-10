/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QTcpSocket>
//--------------------------------------------------------------------------------
#include <QObject>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "tcp_client.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
TCP_Client::TCP_Client(QWidget* parent)
    : MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void TCP_Client::init(void)
{
    tcpSocket = new QTcpSocket();
    connect(tcpSocket,  &QTcpSocket::readyRead,     this,   &TCP_Client::readyData);
    connect(tcpSocket,  &QTcpSocket::disconnected,  this,   &TCP_Client::disconnected);
    connect(tcpSocket,  SIGNAL(error(QAbstractSocket::SocketError)),        this,   SIGNAL(socket_error(QAbstractSocket::SocketError)));
    connect(tcpSocket,  SIGNAL(stateChanged(QAbstractSocket::SocketState)), this,   SIGNAL(state_changed(QAbstractSocket::SocketState)));
}
//--------------------------------------------------------------------------------
void TCP_Client::readyData(void)
{
    emit readyRead();
}
//--------------------------------------------------------------------------------
void TCP_Client::setAddress(const QHostAddress &address)
{
#ifdef DEBUG
    emit debug(QString("set address %1").arg(address.toString()));
#endif
    this->address = address.toString();
}
//--------------------------------------------------------------------------------
void TCP_Client::setPort(unsigned int port)
{
#ifdef DEBUG
    emit debug(QString("set port %1").arg(port));
#endif
    this->port = port;
}
//--------------------------------------------------------------------------------
QByteArray TCP_Client::send_data(const QByteArray block)
{
    QByteArray tmp;

    tmp.clear();
    emit info(QString("connect to host %1:%2").arg(address).arg(port));
    tcpSocket->connectToHost(address,
                             static_cast<quint16>(port));
    if (!tcpSocket->waitForConnected(3000))
    {
        emit error(tr("Сервер не отвечает!"));
        return nullptr;
    }
    tcpSocket->write(block);
    if (tcpSocket->waitForBytesWritten(3000))
    {
        emit info(tr("Данные переданы!"));
    }
    else
    {
        emit error(tr("Данные передать не удалось!"));
    }

    if(tcpSocket->waitForReadyRead (5000))
    {
        tmp = tcpSocket->readAll();
        emit info(tr("Данные получены!"));
    }
    else
    {
        emit error(tr("Данные получить не удалось!"));
    }

    tcpSocket->disconnectFromHost();
    return tmp;
}
//--------------------------------------------------------------------------------
void TCP_Client::connect_to_host(QString address, quint16 port)
{
    emit info(QString("connect to host %1:%2").arg(address).arg(port));
    if(tcpSocket)
    {
        tcpSocket->connectToHost(address, port);
    }
}
//--------------------------------------------------------------------------------
void TCP_Client::disconnect_from_host(void)
{
    if(tcpSocket)
    {
        emit info("disconnect_from_host");
        tcpSocket->disconnectFromHost();
    }
}
//--------------------------------------------------------------------------------
qint64 TCP_Client::write_data(QByteArray data)
{
    if(tcpSocket)
    {
        return tcpSocket->write(data);
    }
    return 0;
}
//--------------------------------------------------------------------------------
QByteArray TCP_Client::readAll(void)
{
    if(tcpSocket == nullptr)
    {
        return nullptr;
    }
    if(tcpSocket->isOpen())
    {
        return tcpSocket->readAll();
    }
    return nullptr;
}
//--------------------------------------------------------------------------------
QString TCP_Client::get_errorString(void)
{
    if(tcpSocket == nullptr)
    {
        return "";
    }
    if(tcpSocket->isOpen())
    {
        return tcpSocket->errorString();
    }
    return "tcpSocket not open";
}
//--------------------------------------------------------------------------------
QTcpSocket::SocketState TCP_Client::get_state(void)
{
    return tcpSocket->state();
}
//--------------------------------------------------------------------------------
QByteArray TCP_Client::input(const QByteArray &data)
{
    return send_data(data);
}
//--------------------------------------------------------------------------------
void TCP_Client::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool TCP_Client::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void TCP_Client::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void TCP_Client::save_setting(void)
{

}
//--------------------------------------------------------------------------------
