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
    save_widgets();
    if(player)
    {
        delete player;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::create_player(void)
{
    //player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    player = new QMediaPlayer(this);
    player->setVolume(0);   //TODO выключить звук
    player->setVideoOutput(ui->video_widget);

    connect(player, SIGNAL(error(QMediaPlayer::Error)), this,   SLOT(f_error(QMediaPlayer::Error)));
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::init(void)
{
    ui->setupUi(this);

    ui->video_widget->setMinimumSize(640, 480);
    ui->sb_port->setRange(0, 0xFFFF);

//    QString ip_string = load_string("IP");
//    if(ip_string.isEmpty()) ip_string = "ip://127.0.0.1";
//    ui->ipv4_widget->set_url(QUrl(ip_string));

    connect(ui->btn_connect,    &QPushButton::clicked,  this,   &Simple_PTZ_widget::play);
    connect(ui->btn_disconnect, &QPushButton::clicked,  this,   &Simple_PTZ_widget::stop);

    connect(&networkManager,    SIGNAL(finished( QNetworkReply*)),  this,   SLOT(onFinished(QNetworkReply*)));

    connect(ui->ipv4_widget,    &IPV4_wo_port::editingFinished, this,   &Simple_PTZ_widget::play);
    connect(ui->le_login,       &QLineEdit::editingFinished,    this,   &Simple_PTZ_widget::play);
    connect(ui->le_password,    &QLineEdit::editingFinished,    this,   &Simple_PTZ_widget::play);
    connect(ui->sb_port,        &QSpinBox::editingFinished,     this,   &Simple_PTZ_widget::play);

    url = ui->ipv4_widget->get_url();
    url.setPort(ui->sb_port->value());
    emit debug(QString("IP %1").arg(url.toString()));

    load_widgets();

    create_player();
    connect_position_widgets();

    ui->btn_d->setDisabled(true);
    ui->btn_l->setDisabled(true);
    ui->btn_r->setDisabled(true);
    ui->btn_u->setDisabled(true);
    ui->btn_up_down->setDisabled(true);
    ui->btn_left_right->setDisabled(true);
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

    connect(ui->btn_left_right, SIGNAL(clicked()),  this,   SLOT(f_left_right()));
    connect(ui->btn_up_down,    SIGNAL(clicked()),  this,   SLOT(f_up_down()));
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
    emit error(QString("Error string  = [%1]").arg(player->errorString()));
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::onFinished( QNetworkReply* reply )
{
    emit trace(Q_FUNC_INFO);
    if(reply->error() == QNetworkReply::NoError)
    {
        QString data = QString::fromUtf8(reply->readAll());
        emit debug(data);
    }
    else
    {
        emit error(reply->errorString());
    }

    reply->deleteLater();
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::play(void)
{
    emit trace(Q_FUNC_INFO);
    if(player->isAvailable())
    {
        player->stop();
#if 1
        QString url_str;
        url_str.append("rtsp://");
        url_str.append(ui->le_login->text());
        url_str.append(":");
        url_str.append(ui->le_password->text());
        url_str.append("@");
        url_str.append(ui->ipv4_widget->get_url().host());
        url_str.append(":");
        url_str.append(QString("%1").arg(ui->sb_port->value()));

        emit debug(url_str);

        //const QUrl url = QUrl("rtsp://admin:admin@192.168.1.14:81");
        url = QUrl(url_str);
        url.setPort(ui->sb_port->value());

        const QNetworkRequest requestRtsp(url);
        player->setMedia(requestRtsp);
        player->setVolume(0);   //TODO установить громкость
        player->play();
#else
        url.setScheme("rtsp");

        emit debug(url.toString());

        QString concatenated = ui->le_login->text() + ":" + ui->le_password->text(); //username:password
        emit debug(QString("concatenated = [%1]").arg(concatenated));

        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;
        QNetworkRequest request(url);
        //QNetworkRequest request(QUrl("http://192.168.1.14:81/videostream.cgi?stream=1"));
        request.setRawHeader("Authorization", headerData.toLocal8Bit());
        request.setRawHeader("Connection:", "keep-alive");

        //qDebug() << request;

        player->setMedia(QMediaContent(request));
        //player->setMedia(request);
        player->play();
#endif
        ui->btn_d->setEnabled(true);
        ui->btn_l->setEnabled(true);
        ui->btn_r->setEnabled(true);
        ui->btn_u->setEnabled(true);
        ui->btn_up_down->setEnabled(true);
        ui->btn_left_right->setEnabled(true);
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

        ui->btn_d->setDisabled(true);
        ui->btn_l->setDisabled(true);
        ui->btn_r->setDisabled(true);
        ui->btn_u->setDisabled(true);
        ui->btn_up_down->setDisabled(true);
        ui->btn_left_right->setDisabled(true);
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
void Simple_PTZ_widget::f_left_right(void)
{
    send_cmd("leftright");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_up_down(void)
{
    send_cmd("updown");
}
//--------------------------------------------------------------------------------
//http://192.168.1.14:81/moveptz.xml?dir=left
//http://192.168.1.14:81/moveptz.xml?dir=right
//http://192.168.1.14:81/moveptz.xml?dir=leftright
//http://192.168.1.14:81/moveptz.xml?dir=updown

//http://192.168.1.14:81/moveptz.xml?dir=down
//http://192.168.1.14:81/videostream.cgi?stream=1

void Simple_PTZ_widget::send_cmd(QString  cmd)
{
    emit trace(Q_FUNC_INFO);

    QString param;
    param.append(QString("http://%1:%2/moveptz.xml?dir=%3")
                 .arg(url.host())
                 .arg(url.port())
                 .arg(cmd));

    QString concatenated = ui->le_login->text() + ":" + ui->le_password->text(); //username:password
    emit info(QString("%1").arg(concatenated));

    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    QNetworkRequest request=QNetworkRequest(QUrl( param ));
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
    request.setRawHeader("Connection:", "keep-alive");
    networkManager.get(request);

    emit info(param);
    emit info("OK");
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
