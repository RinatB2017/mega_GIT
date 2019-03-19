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
#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QWidget>
#include <QString>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class QTcpSocket;
class QByteArray;
class QWidget;
//--------------------------------------------------------------------------------
class TCP_Client : public MyWidget
{
    Q_OBJECT

public:
    TCP_Client(QWidget* parent = nullptr);
    void setAddress(const QHostAddress &);
    void setPort(unsigned int);

signals:
    void output(const QByteArray &);

public slots:
    QByteArray input(const QByteArray &data);

private slots:
    QByteArray send_data(const QByteArray);

private:
    QTcpSocket *tcpSocket;
    QString address;
    uint port = 0;
    void init(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
