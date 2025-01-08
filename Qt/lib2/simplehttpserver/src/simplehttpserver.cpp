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
#include "simplehttpserver.hpp"
//--------------------------------------------------------------------------------
SimpleHttpServer::SimpleHttpServer(QObject *parent) :
    QTcpServer(parent)
{
    connect(this, &QTcpServer::newConnection, this, &SimpleHttpServer::handleNewConnection);
}
//--------------------------------------------------------------------------------
void SimpleHttpServer::handleNewConnection(void)
{
    QTcpSocket *clientSocket = nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {
        QByteArray requestData = clientSocket->readAll();
        QString request(requestData);
        if (request.startsWith("GET /screenshot"))
        {
            sendScreenshot(clientSocket);
        }
        else
        {
            sendNotFound(clientSocket);
        }
    });
}
//--------------------------------------------------------------------------------
void SimpleHttpServer::sendScreenshot(QTcpSocket *clientSocket)
{
    // Получаем указатель на основной экран
    QScreen *screen = QApplication::primaryScreen();
    if (screen)
    {
        // Захватываем изображение всего экрана
        QPixmap screenshot = screen->grabWindow(0);
        QImage image = screenshot.toImage();

        // Сжимаем изображение в формат JPEG
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "JPEG");

        // Формируем HTTP-ответ
        QByteArray response;
        response.append("HTTP/1.1 200 OK\r\n");
        response.append("Content-Type: image/jpeg\r\n");
        response.append("Content-Length: " + QByteArray::number(byteArray.size()) + "\r\n");
        response.append("\r\n");
        response.append(byteArray);

        // Отправляем ответ клиенту
        clientSocket->write(response);
    }
    else
    {
        sendNotFound(clientSocket);
    }
    clientSocket->disconnectFromHost();
}
//--------------------------------------------------------------------------------
void SimpleHttpServer::sendNotFound(QTcpSocket *clientSocket)
{
    QByteArray response = "HTTP/1.1 404 Not Found\r\n\r\n";
    clientSocket->write(response);
    clientSocket->disconnectFromHost();
}
//--------------------------------------------------------------------------------
