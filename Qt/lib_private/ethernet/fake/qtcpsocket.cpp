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
#include <QHostAddress>
#include <QInputDialog>
#include <QMessageBox>
#include <QByteArray>
#include <QObject>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "qtcpsocket.h"
#include "qxmlputget.h"
//--------------------------------------------------------------------------------
#define FAKE_XML "fake.xml"
//--------------------------------------------------------------------------------
QTcpSocket::QTcpSocket(QObject *parent)
{
    struct DATA data;
    QByteArray in;
    QByteArray out;
    QXmlGet xmlGet;
    bool ok;
    QString error_message;
    int error_line;
    int error_column;

    process.clear();

    if(!QFile(FAKE_XML).exists())
    {
        qDebug() << QString("%1 not found !").arg(FAKE_XML);
    }

    ok = xmlGet.load(FAKE_XML, &error_message, &error_line, &error_column);
    if(!ok)
    {
        qDebug() << QString("%1 not load ! %2 %3 %4")
                    .arg(FAKE_XML)
                    .arg(error_message)
                    .arg(error_line)
                    .arg(error_column);
    }

    while(xmlGet.findNext("data"))
    {
        in.clear();
        out.clear();

        xmlGet.descend();
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        if (xmlGet.find("in"))
            in = QByteArray::fromHex(xmlGet.getString("in").toAscii());
        if (xmlGet.find("out"))
            out = QByteArray::fromHex(xmlGet.getString("out").toAscii());
#else
        if (xmlGet.find("in"))
            in = QByteArray::fromHex(xmlGet.getString("in").toLocal8Bit());
        if (xmlGet.find("out"))
            out = QByteArray::fromHex(xmlGet.getString("out").toLocal8Bit());
#endif
        xmlGet.rise();

        data.in  = in;
        data.out = out;
        process.push_back(data);
    }
    qDebug() << "QTcpSocket()";
    qDebug() << QString("load %1").arg(process.count());
    Q_UNUSED(parent)
}
//--------------------------------------------------------------------------------
QTcpSocket::~QTcpSocket()
{
    qDebug() << "~QTcpSocket()";
}
//--------------------------------------------------------------------------------
void QTcpSocket::connectToHost(const QString &hostName, quint16 port, OpenMode mode)
{
    qDebug() << "hostName" << hostName << "port" << port << "mode" << mode;
}
//--------------------------------------------------------------------------------
void QTcpSocket::connectToHost(const QHostAddress &address, quint16 port, OpenMode mode)
{
    qDebug() << "address" << address.toString() << "port" << port << "mode" << mode;
}
//--------------------------------------------------------------------------------
void QTcpSocket::disconnectFromHost(void)
{
    qDebug() << "disconnectFromHost()";
}
//--------------------------------------------------------------------------------
bool QTcpSocket::waitForConnected(int msecs)
{
    qDebug() << QString("waitForConnected(%1)").arg(msecs);
    return true;
}
//--------------------------------------------------------------------------------
bool QTcpSocket::waitForBytesWritten(int msecs)
{
    qDebug() << QString("waitForBytesWritten(%1)").arg(msecs);
    return true;
}
//--------------------------------------------------------------------------------
bool QTcpSocket::waitForReadyRead(int msecs)
{
    qDebug() << QString("waitForReadyRead(%1)").arg(msecs);
    return true;
}
//--------------------------------------------------------------------------------
qint64 QTcpSocket::write(const QByteArray &data)
{
    qDebug() << QString("write(%1)").arg(data.toHex().data());

    input_data.clear();
    input_data.append(data);

    emit readyRead();
    return data.length();
}
//--------------------------------------------------------------------------------
QByteArray QTcpSocket::readAll(void)
{
    qDebug() << "readAll()";
    output_data = processing(input_data);
    return output_data;
}
//--------------------------------------------------------------------------------
QByteArray QTcpSocket::processing(const QByteArray &data)
{
    QByteArray temp;

    // qDebug() << data.toHex();
    if(data.length() > 0)
    {
        if(data == QByteArray("test"))
            return QByteArray("ok");

        for(int i=0; i<process.count(); i++)
        {
            if(process[i].in == data)
            {
                qDebug() << "found!";
                temp = process[i].out;
                return temp;
            }
        }
        QInputDialog *dlg = new QInputDialog();
        dlg->setLabelText(data.toHex());
        int btn = dlg->exec();
        if(btn == QInputDialog::Accepted)
        {
            if(!dlg->textValue().isEmpty())
            {
                struct DATA save_data;
                save_data.in  = data;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
                save_data.out = QByteArray::fromHex(dlg->textValue().toAscii());
#else
                save_data.out = QByteArray::fromHex(dlg->textValue().toLocal8Bit());
#endif
                process.push_back(save_data);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
                return QByteArray::fromHex(dlg->textValue().toAscii());
#else
                return QByteArray::fromHex(dlg->textValue().toLocal8Bit());
#endif
            }
            else
            {
                QMessageBox::warning(0, tr("Ошибка"), tr("Пустое значение не допустимо!"), QMessageBox::Ok);
            }
        }
    }

    temp.clear();
    temp.append(QByteArray::fromHex("010000"));
    return temp;
}
//--------------------------------------------------------------------------------
void QTcpSocket::connectToHostImplementation(QString &hostName, quint16 port, OpenMode mode)
{
    qDebug() << "hostName" << hostName << "port" << port << "mode" << mode;
}
//--------------------------------------------------------------------------------
void QTcpSocket::abort(void)
{

}
//--------------------------------------------------------------------------------
bool QTcpSocket::waitForDisconnected( int msecs )
{
    qDebug() << QString("waitForDisconnected(%1)").arg(msecs);
    return true;
}
//--------------------------------------------------------------------------------
QAbstractSocket::SocketState QTcpSocket::state(void) const
{
    return QAbstractSocket::ConnectedState;
}
//--------------------------------------------------------------------------------
QString QTcpSocket::errorString(void)
{
    return "errorString()";
}
//--------------------------------------------------------------------------------
