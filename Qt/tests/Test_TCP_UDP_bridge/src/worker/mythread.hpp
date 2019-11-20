/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#ifndef MYTHREAD_HPP
#define MYTHREAD_HPP
//--------------------------------------------------------------------------------
#include <QTcpServer>
#include <QUdpSocket>
#include <QDateTime>
#include <QWidget>
//--------------------------------------------------------------------------------
class MyThread : public QObject
{
    Q_OBJECT

public:
    MyThread(QObject *parent = nullptr);
    ~MyThread();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void finished(void);

public slots:
    void process(void);

    void set_port1(quint16 value);
    void set_port2(quint16 value);

    void start(void);
    void stop(void);

private:
    bool flag_exit = false;

    QUdpSocket *udp_socket1 = nullptr;
    QUdpSocket *udp_socket2 = nullptr;

    quint16 port1 = 0;
    quint16 port2 = 0;

    void init(void);
};
//--------------------------------------------------------------------------------
#endif // MYTHREAD_HPP
