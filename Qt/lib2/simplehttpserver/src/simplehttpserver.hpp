/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
#ifndef SIMPLEHTTPSERVER_HPP
#define SIMPLEHTTPSERVER_HPP
//--------------------------------------------------------------------------------
#include <QApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QScreen>
#include <QBuffer>
//--------------------------------------------------------------------------------
// http://localhost:8080/screenshot
// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);
//     SimpleHttpServer server;
//     if (server.listen(QHostAddress::Any, 8080))
//     {
//         qDebug() << "Server started on port 8080";
//     }
//     else
//     {
//         qDebug() << "Failed to start server";
//         return 1;
//     }
//     return app.exec();
// }
//--------------------------------------------------------------------------------
class SimpleHttpServer : public QTcpServer
{
    Q_OBJECT

public:
    SimpleHttpServer(QObject *parent = nullptr);

private slots:
    void handleNewConnection(void);
    void sendScreenshot(QTcpSocket *clientSocket);
    void sendNotFound(QTcpSocket *clientSocket);
};
//--------------------------------------------------------------------------------
#endif // SIMPLEHTTPSERVER_HPP
