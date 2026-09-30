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
#include "ui_tcp_server.h"
//--------------------------------------------------------------------------------
#include "tcp_server.hpp"
//--------------------------------------------------------------------------------
TCP_Server::TCP_Server(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::TCP_Server)
{
    init();
}
//--------------------------------------------------------------------------------
TCP_Server::~TCP_Server()
{
    if(tcpServer)
    {
        delete tcpServer;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
bool TCP_Server::createServerOnPort(const QHostAddress address, quint16 port)
{
    emit info(QString("Создание сервера на порту %1").arg(port));

    if(tcpServer)
    {
        tcpServer->close();
        delete tcpServer;
    }

    tcpServer = new QTcpServer(this);
    connect(tcpServer,  &QTcpServer::acceptError,   this,   &TCP_Server::print_error);

    if (!tcpServer->listen(address, port))
    {
        emit error(QString("Ошибка: %1").arg(tcpServer->errorString()));
        return false;
    }
    emit info("Сервер создан");
    emit info(QString("IP: %1").arg(tcpServer->serverAddress().toString()));
    emit info(QString("Port: %1").arg(tcpServer->serverPort()));

    is_open = true;
    emit server_is_open(is_open);

    connect(tcpServer,  &QTcpServer::newConnection, this,   &TCP_Server::new_connect);
    return true;
}
//--------------------------------------------------------------------------------
void TCP_Server::closeServer()
{
    if(tcpServer)
    {
        tcpServer->close();
    }

    is_open = false;
    emit server_is_open(is_open);
}
//--------------------------------------------------------------------------------
void TCP_Server::new_connect()
{
    while(tcpServer->hasPendingConnections())
    {
        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
        if (!clientSocket) continue;

        emit info(QString("Клиент подключился: %1:%2")
                  .arg(clientSocket->peerAddress().toString())
                  .arg(clientSocket->peerPort()));

        bool client_exist = false;
        foreach (CLIENT client, l_clients) {
            if(client.address == clientSocket->peerAddress())
            {
                client_exist = true;
            }
        }
        if(client_exist == false)
        {
            // новый клиент
            QWidget *client_widget = new QWidget();

            LogBox *log = new LogBox(client_widget);

            CLIENT client;
            client.name = clientSocket->peerAddress().toString();
            client.address = clientSocket->peerAddress();
            client.log = log;

            l_clients.append(client);

            QVBoxLayout *vbox = new QVBoxLayout();
            vbox->addWidget(log);
            client_widget->setLayout(vbox);

            ui->tw_clients->addTab(client_widget, clientSocket->peerAddress().toString());
        }

        // Подписываемся на события конкретного сокета
        connect(clientSocket, &QTcpSocket::readyRead,           this, &TCP_Server::client_ready_read);
        connect(clientSocket, &QAbstractSocket::errorOccurred,  this, &TCP_Server::print_error);
        connect(clientSocket, &QTcpSocket::disconnected,        this, &TCP_Server::client_disconnected);
    }
}
//--------------------------------------------------------------------------------
void TCP_Server::client_disconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    // emit info(QString("Клиент %1 отключился").arg(clientSocket->peerAddress().toString()));
    clientSocket->deleteLater();
}
//--------------------------------------------------------------------------------
void TCP_Server::f_connect()
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
    {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    createServerOnPort(QHostAddress(ipAddress), 1234);
}
//--------------------------------------------------------------------------------
void TCP_Server::f_disconnect()
{
    emit trace(Q_FUNC_INFO);
    closeServer();
}
//--------------------------------------------------------------------------------
void TCP_Server::init()
{
    ui->setupUi(this);

    is_open = false;
    connects();

    ui->tw_clients->clear();

    QTimer::singleShot(0, [this]{
        emit server_is_open(is_open);
    });
}
//--------------------------------------------------------------------------------
void TCP_Server::connects()
{
    connect(ui->btn_create,     &QPushButton::clicked,  this,   &TCP_Server::f_connect);
}
//--------------------------------------------------------------------------------
void TCP_Server::client_ready_read()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    QList<QByteArray> sl = data.split('|');
    int cnt = sl.count();
    if(cnt == 2)
    {
        foreach (CLIENT client, l_clients) {
            if(client.address == clientSocket->peerAddress())
            {
                if(sl.at(0) == "INFO")  client.log->infoLog(sl.at(1));
                if(sl.at(0) == "DEBUG") client.log->debugLog(sl.at(1));
                if(sl.at(0) == "ERROR") client.log->errorLog(sl.at(1));
                if(sl.at(0) == "TRACE") client.log->traceLog(sl.at(1));
                return;
            }
        }
    }
    else
    {
        emit error(QString("Bad count: %1").arg(cnt));
    }
}
//--------------------------------------------------------------------------------
void TCP_Server::print_error(QAbstractSocket::SocketError socketError)
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    if (socketError != QAbstractSocket::RemoteHostClosedError) {
        emit error(QString("Ошибка сокета для IP %1:%2")
                   .arg(clientSocket->peerAddress().toString())
                   .arg(clientSocket->errorString()));
    }
}
//--------------------------------------------------------------------------------
bool TCP_Server::is_opened()
{
    return is_open;
}
//--------------------------------------------------------------------------------
void TCP_Server::updateText()
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool TCP_Server::programm_is_exit()
{
    return true;
}
//--------------------------------------------------------------------------------
void TCP_Server::load_setting()
{

}
//--------------------------------------------------------------------------------
void TCP_Server::save_setting()
{

}
//--------------------------------------------------------------------------------
