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
#ifdef DEBUG_FRAME
    emit debug(block.toHex().toUpper());
#endif
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
        //emit trace(tmp.data());
    }
    else
    {
        emit error(tr("Данные получить не удалось!"));
    }

    tcpSocket->disconnectFromHost();
    return tmp;
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
