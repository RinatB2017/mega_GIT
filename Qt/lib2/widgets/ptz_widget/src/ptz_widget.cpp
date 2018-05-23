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
//--------------------------------------------------------------------------------
#include "ptz_widget.hpp"
#include "ptz_dialog.hpp"
//--------------------------------------------------------------------------------
#include "for_tests_mainbox.hpp"
#include "ui_ptz_widget.h"
//--------------------------------------------------------------------------------
PTZ_widget::PTZ_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PTZ_widget)
{
    init();
}
//--------------------------------------------------------------------------------
PTZ_widget::~PTZ_widget()
{
    if(player)
    {
        player->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void PTZ_widget::init(void)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    player->setVideoOutput(ui->video_widget);

    connect(player, SIGNAL(error(QMediaPlayer::Error)), this,   SLOT(f_error(QMediaPlayer::Error)));

    ui->video_widget->setMinimumSize(320, 200);

    //---
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,  SIGNAL(readyRead()),    this,   SLOT(read_data()));
    connect(tcpSocket,  SIGNAL(error(QAbstractSocket::SocketError)), this, (SLOT(s_error(QAbstractSocket::SocketError))));
    connect(tcpSocket,  SIGNAL(connected()),    this,   SLOT(f_connected()));
    connect(tcpSocket,  SIGNAL(disconnected()), this,   SLOT(f_disconnected()));
    //---
    btn_lu = new QToolButton(ui->video_widget);
    btn_ru = new QToolButton(ui->video_widget);

    btn_u  = new QToolButton(ui->video_widget);
    btn_d  = new QToolButton(ui->video_widget);
    btn_l  = new QToolButton(ui->video_widget);
    btn_r  = new QToolButton(ui->video_widget);

    btn_ld = new QToolButton(ui->video_widget);
    btn_rd = new QToolButton(ui->video_widget);

    btn_lu->setFixedSize(32, 32);
    btn_ru->setFixedSize(32, 32);
    btn_u->setFixedSize(32, 32);
    btn_d->setFixedSize(32, 32);
    btn_l->setFixedSize(32, 32);
    btn_r->setFixedSize(32, 32);
    btn_ld->setFixedSize(32, 32);
    btn_rd->setFixedSize(32, 32);

    btn_lu->setIcon(QIcon(":/arrows/up_left.png"));
    btn_ru->setIcon(QIcon(":/arrows/up_right.png"));

    btn_u->setIcon(QIcon(":/arrows/up.png"));
    btn_d->setIcon(QIcon(":/arrows/down.png"));
    btn_l->setIcon(QIcon(":/arrows/left.png"));
    btn_r->setIcon(QIcon(":/arrows/right.png"));

    btn_ld->setIcon(QIcon(":/arrows/down_left.png"));
    btn_rd->setIcon(QIcon(":/arrows/down_right.png"));

    //---
    ui->sl_speed->setRange(0, 100);
    ui->sl_speed->setValue(50);
    //---

    connect(btn_lu, SIGNAL(pressed()),  this,   SLOT(f_left_up()));
    connect(btn_ru, SIGNAL(pressed()),  this,   SLOT(f_right_up()));
    connect(btn_l,  SIGNAL(pressed()),  this,   SLOT(f_left()));
    connect(btn_r,  SIGNAL(pressed()),  this,   SLOT(f_right()));
    connect(btn_u,  SIGNAL(pressed()),  this,   SLOT(f_up()));
    connect(btn_d,  SIGNAL(pressed()),  this,   SLOT(f_down()));
    connect(btn_ld, SIGNAL(pressed()),  this,   SLOT(f_left_down()));
    connect(btn_rd, SIGNAL(pressed()),  this,   SLOT(f_right_down()));

    connect(btn_lu, SIGNAL(released()), this,   SLOT(f_stop()));
    connect(btn_ru, SIGNAL(released()), this,   SLOT(f_stop()));
    connect(btn_l,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(btn_r,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(btn_u,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(btn_d,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(btn_ld, SIGNAL(released()), this,   SLOT(f_stop()));
    connect(btn_rd, SIGNAL(released()), this,   SLOT(f_stop()));
    //---

#ifdef Q_OS_LINUX
    ui->le_address->setText("rtsp://192.168.1.66/av0_0");
#else
    ui->le_address->setText("rtsp://192.168.1.88/HD");
#endif
    ui->le_address->setEnabled(false);

    //---
    QByteArray ba;
    ba.append(ui->le_address->text());
    url = QUrl::fromEncoded(ba);
    url.setPort(80);
    emit info(QString("IP %1").arg(url.host()));
    //---

    ui->btn_play->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_pause->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPause));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    connect(ui->btn_play,   SIGNAL(clicked(bool)),  this,   SLOT(play()));
    connect(ui->btn_pause,  SIGNAL(clicked(bool)),  this,   SLOT(pause()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(stop()));

    connect(ui->btn_choice, SIGNAL(clicked(bool)),  this,   SLOT(choice()));
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_error(QMediaPlayer::Error err)
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

    default:
        emit error(QString("unknown error %1").arg(err));
        break;
    }
    emit error(player->errorString());
}
//--------------------------------------------------------------------------------
void PTZ_widget::play(void)
{
    if(player->isAvailable())
    {
        player->stop();

        const QUrl url = QUrl(ui->le_address->text());
        const QNetworkRequest requestRtsp(url);
        player->setMedia(requestRtsp);
        player->play();
    }
    else
    {
        emit error("Player is not available!");
    }
}
//--------------------------------------------------------------------------------
void PTZ_widget::pause(void)
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
void PTZ_widget::stop(void)
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
void PTZ_widget::choice(void)
{
    PTZ_dialog *dlg = new PTZ_dialog;
    dlg->set_url(QUrl("192.168.1.66"));
    int btn = dlg->exec();
    if(btn == PTZ_dialog::Accepted)
    {
        emit debug(QString("[%1]").arg(dlg->get_address()));
        ui->le_address->setText(dlg->get_address());

        QByteArray ba;
        ba.append(ui->le_address->text());
        url = QUrl::fromEncoded(ba);
        url.setPort(80);
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void PTZ_widget::resizeEvent(QResizeEvent *)
{
    int lu_x = 0;
    int lu_y = 0;

    int ru_x = ui->video_widget->width() - btn_ru->width();
    int ru_y = 0;

    int ld_x = 0;
    int ld_y = ui->video_widget->height() - btn_ld->height();

    int rd_x = ui->video_widget->width() - btn_rd->width();
    int rd_y = ui->video_widget->height() - btn_rd->height();

    int u_x = ui->video_widget->width() / 2.0 - btn_u->width() / 2.0;
    int u_y = 0;

    int d_x = ui->video_widget->width() / 2.0 - btn_d->width() / 2.0;
    int d_y = ui->video_widget->height() - btn_d->height();

    int l_x = 0;
    int l_y = ui->video_widget->height() / 2.0 - btn_l->height() / 2.0;

    int r_x = ui->video_widget->width() - btn_r->width();
    int r_y = ui->video_widget->height() / 2.0 - btn_r->height() / 2.0;

    btn_lu->move(lu_x, lu_y);
    btn_ru->move(ru_x, ru_y);

    btn_u->move(u_x, u_y);
    btn_d->move(d_x, d_y);
    btn_l->move(l_x, l_y);
    btn_r->move(r_x, r_y);

    btn_ld->move(ld_x, ld_y);
    btn_rd->move(rd_x, rd_y);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_stop(void)
{
    send_cmd("STOP", 0, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left_up(void)
{
    send_cmd("LU", ui->sl_speed->value(), ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right_up(void)
{
    send_cmd("RU", ui->sl_speed->value(), ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left(void)
{
    send_cmd("L", ui->sl_speed->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right(void)
{
    send_cmd("R", ui->sl_speed->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_up(void)
{
    send_cmd("U", 0, ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_down(void)
{
    send_cmd("D", 0, ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left_down(void)
{
    send_cmd("LD", ui->sl_speed->value(), ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right_down(void)
{
    send_cmd("RD", ui->sl_speed->value(), ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::send_cmd(QString cmd, int speed_x, int speed_y)
{
    emit trace(Q_FUNC_INFO);

    emit debug(QString("cmd %1").arg(cmd));
    emit debug(QString("speed_x %1").arg(speed_x));
    emit debug(QString("speed_y %1").arg(speed_y));

    QString param;
    param.append("http://");
    param.append(url.host());
    param.append("/cgi-bin/senddata.cgi?cmd=ptz;func=");
    param.append(cmd);
    param.append(QString(";value1=%1;").arg(speed_x));
    param.append(QString(";value2=%1;").arg(speed_y));

    //---
    bool ok = false;

    ok = tcpSocket->isOpen();
    if(ok == false)
    {
        emit debug("isOpen() return false");
        ok = f_connect();
        if(ok == false)
        {
            return;
        }
    }

    QByteArray reqStr;
    reqStr.append(QString("GET %1 HTTP/1.1\r\n")
                  .arg(param));

    reqStr.append(QString("Host: %1\r\n").arg(url.host()));
    reqStr.append("User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5\r\n");
    reqStr.append("Accept: text/html\r\n");
    reqStr.append("Connection: close\r\n");
    reqStr.append("\r\n");

    qint64 bytes = tcpSocket->write(reqStr);
    if(bytes < 0)
    {
        emit error(QString("write bytes %1").arg(bytes));
        return;
    }
    ok = tcpSocket->waitForBytesWritten(1000);
    if(!ok)
    {
        emit error("waitForBytesWritten");
        return;
    }
    ok = tcpSocket->waitForReadyRead(1000);
    if(!ok)
    {
        emit error("waitForReadyRead");
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
bool PTZ_widget::f_connect(void)
{
    QString ip = url.host();
    if(ip.isEmpty())
    {
        emit error("ip is empty!");
        return false;
    }
    int port = url.port();

    emit debug(QString("%1:%2")
               .arg(ip)
               .arg(port));

    tcpSocket->connectToHost(ip, port);
    return true;
}
//--------------------------------------------------------------------------------
void PTZ_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
