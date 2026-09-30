/*********************************************************************************
**                                                                              **
**     Copyright (C) 2026                                                       **
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
#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP
//--------------------------------------------------------------------------------
#include <QNetworkInterface>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
//--------------------------------------------------------------------------------
#include "logbox.hpp"
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class TCP_Server;
}
//--------------------------------------------------------------------------------
class TCP_Server : public MyWidget
{
    Q_OBJECT

public:
    explicit TCP_Server(QWidget *parent = nullptr);
    virtual ~TCP_Server();

    bool is_opened();

signals:
    void server_is_open(bool);

public slots:    
    bool createServerOnPort(const QHostAddress address, quint16 port);
    void closeServer();

private slots:
    void new_connect();
    void client_ready_read();
    void client_disconnected();

    void f_connect();
    void f_disconnect();

private:
    Ui::TCP_Server *ui;
    QTcpServer *tcpServer = nullptr;
    bool is_open = false;

    typedef struct CLIENT
    {
        QString name;
        QHostAddress address;
        LogBox *log;
    } CLIENT_t;

    QList<CLIENT> l_clients;

    void init();
    void connects();

    void print_error(QAbstractSocket::SocketError socketError);    

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif
