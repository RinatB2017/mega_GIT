/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
#include <QNetworkReply>
//--------------------------------------------------------------------------------
#include "simple_ptz_widget.hpp"
//--------------------------------------------------------------------------------
#include "ui_simple_ptz_widget.h"
#include "defines.hpp"
//--------------------------------------------------------------------------------
Simple_PTZ_widget::Simple_PTZ_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Simple_PTZ_widget)
{
    init();
}
//--------------------------------------------------------------------------------
Simple_PTZ_widget::~Simple_PTZ_widget()
{
    save_string("IP", ui->ipv4_widget->get_url().host());
    save_widgets(APPNAME);
    if(player)
    {
        player->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::create_player(void)
{
    player = new QMediaPlayer(this);
    player->setVolume(0);   //TODO выключить звук
    player->setVideoOutput(ui->video_widget);

    connect(player, SIGNAL(error(QMediaPlayer::Error)), this,   SLOT(f_error(QMediaPlayer::Error)));
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::create_tcp_socket(void)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,  SIGNAL(readyRead()),    this,   SLOT(read_data()));
    connect(tcpSocket,  SIGNAL(error(QAbstractSocket::SocketError)), this, (SLOT(s_error(QAbstractSocket::SocketError))));
    connect(tcpSocket,  SIGNAL(connected()),    this,   SLOT(f_connected()));
    connect(tcpSocket,  SIGNAL(disconnected()), this,   SLOT(f_disconnected()));
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::init(void)
{
    ui->setupUi(this);

    create_tcp_socket();
    create_player();
    connect_position_widgets();

    ui->video_widget->setMinimumSize(640, 480);
    ui->sb_port->setRange(0, 0xFFFF);
    ui->ipv4_widget->set_url(QUrl(load_string("IP")));

    connect(ui->btn_run,        SIGNAL(clicked()),                  this,   SLOT(play()));
    connect(&networkManager,    SIGNAL(finished( QNetworkReply*)),  this,   SLOT(onFinished(QNetworkReply*)));

    url = ui->ipv4_widget->get_url();
    url.setPort(port);
    emit debug(QString("IP %1").arg(url.toString()));

    load_widgets(APPNAME);

    play();
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::connect_position_widgets(void)
{
    connect(ui->btn_l,  SIGNAL(pressed()),  this,   SLOT(f_left()));
    connect(ui->btn_r,  SIGNAL(pressed()),  this,   SLOT(f_right()));
    connect(ui->btn_u,  SIGNAL(pressed()),  this,   SLOT(f_up()));
    connect(ui->btn_d,  SIGNAL(pressed()),  this,   SLOT(f_down()));

    connect(ui->btn_l,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_r,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_u,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_d,  SIGNAL(released()), this,   SLOT(f_stop()));
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_error(QMediaPlayer::Error err)
{
    switch (err)
    {
    case QMediaPlayer::NoError:             emit error("NoError");              break;
    case QMediaPlayer::ResourceError:       emit error("ResourceError");        break;
    case QMediaPlayer::FormatError:         emit error("FormatError");          break;
    case QMediaPlayer::NetworkError:        emit error("NetworkError");         break;
    case QMediaPlayer::AccessDeniedError:   emit error("AccessDeniedError");    break;
    case QMediaPlayer::ServiceMissingError: emit error("ServiceMissingError");  break;
    case QMediaPlayer::MediaIsPlaylist:     emit error("MediaIsPlaylist");      break;
    }
    emit error(player->errorString());
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::s_state(QAbstractSocket::SocketState state)
{
    switch (state)
    {
    case QAbstractSocket::UnconnectedState: emit error("UnconnectedState"); break;
    case QAbstractSocket::HostLookupState:  emit error("HostLookupState");  break;
    case QAbstractSocket::ConnectingState:  emit error("ConnectingState");  break;
    case QAbstractSocket::ConnectedState:   emit error("ConnectedState");   break;
    case QAbstractSocket::BoundState:       emit error("BoundState");       break;
    case QAbstractSocket::ListeningState:   emit error("ListeningState");   break;
    case QAbstractSocket::ClosingState:     emit error("ClosingState");     break;
    }
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::s_error(QAbstractSocket::SocketError err)
{
    switch (err)
    {
    case QAbstractSocket::ConnectionRefusedError:           emit error("ConnectionRefusedError");           break;
    case QAbstractSocket::RemoteHostClosedError:            emit error("RemoteHostClosedError");            break;
    case QAbstractSocket::HostNotFoundError:                emit error("HostNotFoundError");                break;
    case QAbstractSocket::SocketAccessError:                emit error("SocketAccessError");                break;
    case QAbstractSocket::SocketResourceError:              emit error("SocketResourceError");              break;
    case QAbstractSocket::SocketTimeoutError:               emit error("SocketTimeoutError");               break;
    case QAbstractSocket::DatagramTooLargeError:            emit error("DatagramTooLargeError");            break;
    case QAbstractSocket::NetworkError:                     emit error("NetworkError");                     break;
    case QAbstractSocket::AddressInUseError:                emit error("AddressInUseError");                break;
    case QAbstractSocket::SocketAddressNotAvailableError:   emit error("SocketAddressNotAvailableError");   break;
    case QAbstractSocket::UnsupportedSocketOperationError:  emit error("UnsupportedSocketOperationError");  break;
    case QAbstractSocket::UnfinishedSocketOperationError:   emit error("UnfinishedSocketOperationError");   break;
    case QAbstractSocket::ProxyAuthenticationRequiredError: emit error("ProxyAuthenticationRequiredError"); break;
    case QAbstractSocket::SslHandshakeFailedError:          emit error("SslHandshakeFailedError");          break;
    case QAbstractSocket::ProxyConnectionRefusedError:      emit error("ProxyConnectionRefusedError");      break;
    case QAbstractSocket::ProxyConnectionClosedError:       emit error("ProxyConnectionClosedError");       break;
    case QAbstractSocket::ProxyConnectionTimeoutError:      emit error("ProxyConnectionTimeoutError");      break;
    case QAbstractSocket::ProxyNotFoundError:               emit error("ProxyNotFoundError");               break;
    case QAbstractSocket::ProxyProtocolError:               emit error("ProxyProtocolError");               break;
    case QAbstractSocket::OperationError:                   emit error("OperationError");                   break;
    case QAbstractSocket::SslInternalError:                 emit error("SslInternalError");                 break;
    case QAbstractSocket::SslInvalidUserDataError:          emit error("SslInvalidUserDataError");          break;
    case QAbstractSocket::TemporaryError:                   emit error("TemporaryError");                   break;
    case QAbstractSocket::UnknownSocketError:               emit error("UnknownSocketError");               break;
    }
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::onFinished( QNetworkReply* reply )
{
    if( reply->error() == QNetworkReply::NoError )
    {
        QString data = QString::fromUtf8( reply->readAll() );
        emit info(data);
    }
    else
    {
        emit error( reply->errorString() );
    }

    // Мы сами должны освободить память для reply
    // Однако делать это через delete нельзя
    reply->deleteLater();
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::read_data(void)
{

}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_connected(void)
{
    emit info("Connected");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_disconnected(void)
{
    emit info("Disconnected");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::play(void)
{
    if(player->isAvailable())
    {
        player->stop();

        url.setScheme("rtsp");

        qDebug() << url;

        QString concatenated = ui->le_login->text() + ":" + ui->le_password->text(); //username:password
        emit debug(QString("concatenated = [%1]").arg(concatenated));

        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;
        QNetworkRequest request(url);
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        //qDebug() << request;

        player->setMedia(request);
        player->play();
    }
    else
    {
        emit error("Player is not available!");
    }
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::pause(void)
{
    if(player->isAvailable())
    {
        player->pause();
    }
    else
    {
        emit error("Player is not available!");
    }
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::stop(void)
{
    if(player->isAvailable())
    {
        player->stop();
    }
    else
    {
        emit error("Player is not available!");
    }
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_stop(void)
{
    send_cmd("stop");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_left(void)
{
    send_cmd("left");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_right(void)
{
    send_cmd("right");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_up(void)
{
    send_cmd("up");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_down(void)
{
    send_cmd("down");
}
//--------------------------------------------------------------------------------
//http://192.168.1.14:81/moveptz.xml?dir=left
//http://192.168.1.14:81/moveptz.xml?dir=right
//http://192.168.1.14:81/moveptz.xml?dir=leftright
//http://192.168.1.14:81/moveptz.xml?dir=updown
void Simple_PTZ_widget::send_cmd(QString  cmd)
{
    emit trace(Q_FUNC_INFO);

    QString param;
    param.append(QString("http://%1:%2/moveptz.xml?dir=%3")
                 .arg(url.host())
                 .arg(port)
                 .arg(cmd));

    QNetworkAccessManager m_manager;

    QString concatenated = ui->le_login->text() + ":" + ui->le_password->text(); //username:password
    emit info(QString("concatenated = [%1]").arg(concatenated));

    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    QNetworkRequest request=QNetworkRequest(QUrl( param ));
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
    m_manager.get(request);

    emit trace(QString("param = %1").arg(param));

//    bool ok = false;
//    ok = tcpSocket->isOpen();
//    if(ok == false)
//    {
//        ok = f_connect();
//        if(ok == false)
//        {
//            return;
//        }
//    }

//    QByteArray reqStr;
//    reqStr.append(QString("GET %1 HTTP/1.1\r\n")
//                  .arg(param));

//    reqStr.append(QString("Host: %1\r\n").arg(url.host()));
//    reqStr.append("User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n");
//    reqStr.append("Accept: */*\r\n");
//    reqStr.append("Referer: http://192.168.1.14:81/stream.htm\r\n");
//    //reqStr.append("Connection: close\r\n");
//    reqStr.append("\r\n");

//    qint64 bytes = tcpSocket->write(reqStr);
//    if(bytes < 0)
//    {
//        emit error(QString("write bytes %1").arg(bytes));
//        f_disconnect();
//        return;
//    }
//    ok = tcpSocket->waitForBytesWritten(1000);
//    if(!ok)
//    {
//        emit error("waitForBytesWritten");
//        f_disconnect();
//        return;
//    }
//#if 0
//    ok = tcpSocket->waitForReadyRead(1000);
//    if(!ok)
//    {
//        emit error("waitForReadyRead");
//        f_disconnect();
//        return;
//    }
//#endif
//    f_disconnect();

    emit info("OK");
}
//--------------------------------------------------------------------------------
bool Simple_PTZ_widget::f_connect(void)
{
    QString ip = url.host();
    if(ip.isEmpty())
    {
        emit error("ip is empty!");
        return false;
    }
    int port = url.port();

    emit debug(QString("f_connect: %1:%2")
               .arg(ip)
               .arg(port));

    tcpSocket->connectToHost(ip, static_cast<quint16>(port));
    return true;
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_disconnect(void)
{
    if(tcpSocket->isOpen())
    {
        tcpSocket->close();
    }
    tcpSocket->disconnectFromHost();
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Simple_PTZ_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
