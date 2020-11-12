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
//#include <QMediaPlayer>
//#include <QMultimedia>
//#include <QtMultimediaWidgets>
//--------------------------------------------------------------------------------
#include "rtsp_widget.hpp"
#include "rtsp_dialog.hpp"
//--------------------------------------------------------------------------------
#include "ui_rtsp_widget.h"
//--------------------------------------------------------------------------------
RTSP_widget::RTSP_widget(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::RTSP_widget)
{
    init();
}
//--------------------------------------------------------------------------------
RTSP_widget::~RTSP_widget()
{
    save_widgets();
    save_setting();
    if(player)
    {
        delete player;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void RTSP_widget::init(void)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    player->setVideoOutput(ui->video_widget);

    connect(player, SIGNAL(error(QMediaPlayer::Error)), this,   SLOT(f_error(QMediaPlayer::Error)));
    
    ui->video_widget->setMinimumSize(320, 200);

    login = "admin";
    password = "admin";
    param = "";
    url.setHost("192.168.1.12");
    url.setPort(80);

    ui->le_address->setEnabled(false);
    ui->le_address->setProperty(NO_SAVE, true);

    ui->btn_play->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_pause->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPause));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    connect(ui->btn_play,   SIGNAL(clicked(bool)),  this,   SLOT(play()));
    connect(ui->btn_pause,  SIGNAL(clicked(bool)),  this,   SLOT(pause()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(stop()));

    connect(ui->btn_choice, SIGNAL(clicked(bool)),  this,   SLOT(choice()));

    load_widgets();
    load_setting();
}
//--------------------------------------------------------------------------------
void RTSP_widget::set_address(const QString &login,
                              const QString &password,
                              const QString &ip,
                              const QString &port)
{
    QString address = QString("rtsp://%1:%2@%3:%4")
            .arg(login)
            .arg(password)
            .arg(ip)
            .arg(port);

    this->login = login;
    this->password = password;
    url.setUrl(QString("ip://%1:%2")
               .arg(ip)
               .arg(port));

    ui->le_address->setText(address);
}
//--------------------------------------------------------------------------------
void RTSP_widget::f_error(QMediaPlayer::Error err)
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
void RTSP_widget::play(void)
{
    if(player->isAvailable())
    {
        player->stop();

        const QUrl url = QUrl(ui->le_address->text());
        const QNetworkRequest requestRtsp(url);
        player->setMedia(requestRtsp);
        player->setVolume(0);   //TODO установить громкость
        player->play();
    }
    else
    {
        emit error("Player is not available!");

        QMediaPlayer::MediaStatus status = player->mediaStatus();
        switch (status)
        {
        case QMediaPlayer::UnknownMediaStatus:    emit error("UnknownMediaStatus");    break;
        case QMediaPlayer::NoMedia:               emit error("NoMedia");          break;
        case QMediaPlayer::LoadingMedia:          emit error("LoadingMedia");     break;
        case QMediaPlayer::LoadedMedia:           emit error("LoadedMedia");      break;
        case QMediaPlayer::StalledMedia:          emit error("StalledMedia");     break;
        case QMediaPlayer::BufferingMedia:        emit error("BufferingMedia");   break;
        case QMediaPlayer::BufferedMedia:         emit error("BufferedMedia");    break;
        case QMediaPlayer::EndOfMedia:            emit error("EndOfMedia");       break;
        case QMediaPlayer::InvalidMedia:          emit error("InvalidMedia");     break;
        }
    }
}
//--------------------------------------------------------------------------------
void RTSP_widget::pause(void)
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
void RTSP_widget::stop(void)
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
void RTSP_widget::choice(void)
{
    RTSP_dialog *dlg = new RTSP_dialog;
    dlg->set_login(login);
    dlg->set_password(password);
    dlg->set_url(url);
    dlg->set_param(param);
    int btn = dlg->exec();
    if(btn == RTSP_dialog::Accepted)
    {
        emit debug(QString("[%1]").arg(dlg->get_address()));
        login = dlg->get_login();
        password = dlg->get_password();
        param = dlg->get_param();
        url = dlg->get_url();

        ui->le_address->setText(dlg->get_address());
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void RTSP_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool RTSP_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void RTSP_widget::load_setting(void)
{
    login = load_string(P_LOGIN);
    password = load_string(P_PASSWORD);
    url.setHost(load_string(P_HOST));

    int t_port = 0;
    if(load_int(P_PORT, &t_port))
    {
        url.setPort(t_port);
    }

    //---
    QString address;
    address.append("rtsp://");
    address.append(login);
    address.append(":");
    address.append(password);
    address.append("@");
    address.append(QString("%1:%2")
                   .arg(url.host())
                   .arg(url.port()));
    if(param.isEmpty() == false)
    {
        address.append("/");
        address.append(param);
    }
    ui->le_address->setText(address);
    //---
}
//--------------------------------------------------------------------------------
void RTSP_widget::save_setting(void)
{
    save_string(P_LOGIN,    login);
    save_string(P_PASSWORD, password);
    save_string(P_HOST,     url.host());
    save_int(P_PORT,        url.port());
}
//--------------------------------------------------------------------------------
