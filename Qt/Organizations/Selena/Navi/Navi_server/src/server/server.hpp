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
#ifndef SERVER_H
#define SERVER_H
//--------------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QQueue>
#include <QWidget>
//--------------------------------------------------------------------------------
class Terminal;
class Proto_NMEA_0183;
class UDP_Server;
//--------------------------------------------------------------------------------
class Server : public QWidget
{
    Q_OBJECT
public:
    explicit Server(QWidget *parent = 0);
    void run_tests(void);
    
signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void send_data(const QString &);

public slots:
    void get_data(const QString &data);
    
private slots:
    void get_rawdata(const QByteArray &data);
    void get_data(const QByteArray &data);
    void log(const QString &data);

private:
    QWidget *parent;
    Terminal *terminal;
    UDP_Server *server;
    QQueue<unsigned char> queue;
    QByteArray message;
    Proto_NMEA_0183 *proto;
    QString address;
    unsigned int port;

    void init(void);
    void connect_log(void);

    void raw_data(QByteArray data);
};
//--------------------------------------------------------------------------------
#endif
