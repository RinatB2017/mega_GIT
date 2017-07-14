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
#include <QStringList>
#include <QSettings>
//--------------------------------------------------------------------------------
#include "proto_NMEA_0183.hpp"
#include "udp_server.hpp"
#include "terminal.hpp"
#include "server.hpp"
//--------------------------------------------------------------------------------
Server::Server(QWidget *parent) :
    QWidget(parent),
    parent(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Server::connect_log(void)
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
void Server::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void Server::raw_data(QByteArray data)
{
    int n;
    bool found;

    for(n=0; n<data.length(); n++) queue.enqueue(data.at(n));
    found = false;
    while(queue.isEmpty() == false)
    {
        if(queue.value(0) == '$')
        {
            found = true;
            break;
        }
        else
        {
            queue.removeAt(0);
        }
    }
    if(found == false)
        return;

    found = false;
    for(n=0; n<queue.size(); n++)
    {
        if(queue.value(n) == '\n')
        {
            found = true;
            break;
        }
    }
    if(found == false)
        return;

    message.clear();
    while(queue.isEmpty() == false)
    {
        if(queue.value(0) != '\n')
        {
            message.append(queue.at(0));
            queue.removeAt(0);
        }
        else
        {
            queue.removeAt(0);
            break;
        }
    }

    emit send_data(message);
}
//--------------------------------------------------------------------------------
void Server::get_rawdata(const QByteArray &data)
{
    raw_data(data);
}
//--------------------------------------------------------------------------------
void Server::init(void)
{
    connect_log();

    terminal = new Terminal("terminal.ini", this);
    connect(terminal, SIGNAL(getdata(QByteArray)), this, SLOT(get_rawdata(QByteArray)));

    proto = new Proto_NMEA_0183(this);

    QSettings *settings = new QSettings("server.ini", QSettings::IniFormat);
    address = settings->value("Address", "127.0.0.1").toString();
    port    = settings->value("Port", 10000).toInt();
    settings->endGroup();
    settings->deleteLater();

    server = new UDP_Server(this);
    server->createServerOnPort(QHostAddress(address), port);
    connect(server, SIGNAL(send(QByteArray)), this, SLOT(get_data(QByteArray)));
}
//--------------------------------------------------------------------------------
void Server::get_data(const QByteArray &data)
{
    terminal->send(data);
}
//--------------------------------------------------------------------------------
void Server::run_tests(void)
{
    proto->run_self_tests();
}
//--------------------------------------------------------------------------------
void Server::get_data(const QString &data)
{
    QByteArray ba;
    int res;

    res = proto->check_message(data);
    if(res == E_NO_ERROR)
    {
        ba.clear();
        ba.append(data);
        terminal->send(ba);
    }
}
//--------------------------------------------------------------------------------
