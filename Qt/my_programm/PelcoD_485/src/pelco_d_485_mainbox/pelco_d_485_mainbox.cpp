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
#include <QMediaPlayer>
#include <QNetworkRequest>
#include <QVideoWidget>
//--------------------------------------------------------------------------------
#include "ui_pelco_d_485_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "serialbox5.hpp"
#include "pelco_d_485_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "crc.h"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();

    if(player)
    {
        player->disconnect();
        player->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    ui->serial_widget->set_caption("RS485");
    ui->serial_widget->add_menu(2);
    //ui->serial_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    //connect(ui->sb_sync,    SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_address, SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_cmd1,    SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_cmd2,    SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_data1,   SIGNAL(editingFinished()),  this,   SLOT(refresh()));
    connect(ui->sb_data2,   SIGNAL(editingFinished()),  this,   SLOT(refresh()));

    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->sb_address, &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->sb_chk_sum, &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->sb_cmd1,    &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->sb_cmd2,    &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->sb_data1,   &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->sb_data2,   &QSpinBox::setEnabled);

    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_up,         &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_down,       &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_left,       &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_right,      &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_run,        &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_send,       &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_up,         &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_down,       &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_up_left,    &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_up_right,   &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_down_left,  &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_down_right, &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_zoom_in,    &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->btn_zoom_out,   &QSpinBox::setEnabled);
    connect(ui->serial_widget,  &SerialBox5::port_is_active,    ui->sl_speed,       &QSpinBox::setEnabled);

    //---
    player = new QMediaPlayer(this);
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this,   SLOT(f_error(QMediaPlayer::Error)));
    player->setVideoOutput(ui->video_widget);

    ui->cb_address->addItem("rtsp://192.168.0.66/av0_0");
    ui->cb_address->addItem("rtsp://192.168.1.66/av0_0");
    ui->cb_address->addItem("rtsp://192.168.1.88:554/HD");
    ui->cb_address->addItem("rtsp://192.168.1.67:554/av0_0");

    connect(ui->btn_run,    SIGNAL(clicked(bool)),  this,   SLOT(f_video()));
    //---
    ui->btn_up_left->setProperty("button",      "up_left");
    ui->btn_up->setProperty("button",           "up");
    ui->btn_up_right->setProperty("button",     "up_right");
    ui->btn_left->setProperty("button",         "left");
    ui->btn_right->setProperty("button",        "right");
    ui->btn_down_left->setProperty("button",    "down_left");
    ui->btn_down->setProperty("button",         "down");
    ui->btn_down_right->setProperty("button",   "down_right");

    ui->btn_zoom_in->setProperty("button",      "zoom_in");
    ui->btn_zoom_out->setProperty("button",     "zoom_out");

    connect(ui->btn_up_left,    SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_up_left,    SIGNAL(released()), this,   SLOT(released()));
    connect(ui->btn_up,         SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_up,         SIGNAL(released()), this,   SLOT(released()));
    connect(ui->btn_up_right,   SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_up_right,   SIGNAL(released()), this,   SLOT(released()));
    connect(ui->btn_left,       SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_left,       SIGNAL(released()), this,   SLOT(released()));
    connect(ui->btn_right,      SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_right,      SIGNAL(released()), this,   SLOT(released()));
    connect(ui->btn_down_left,  SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_down_left,  SIGNAL(released()), this,   SLOT(released()));
    connect(ui->btn_down,       SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_down,       SIGNAL(released()), this,   SLOT(released()));
    connect(ui->btn_down_right, SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_down_right, SIGNAL(released()), this,   SLOT(released()));

    connect(ui->btn_zoom_in,    SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_zoom_in,    SIGNAL(released()), this,   SLOT(released()));
    connect(ui->btn_zoom_out,   SIGNAL(pressed()),  this,   SLOT(pressed()));
    connect(ui->btn_zoom_out,   SIGNAL(released()), this,   SLOT(released()));
    //---

    connect(this,               SIGNAL(send(QByteArray)),   ui->serial_widget,  SLOT(input(QByteArray)));
    connect(ui->serial_widget,  SIGNAL(output(QByteArray)), this,               SLOT(read_data(QByteArray)));

    connect(ui->btn_send,   SIGNAL(clicked(bool)),  this,   SLOT(f_send()));

    load_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = reinterpret_cast<MainWindow *>(parentWidget());
    Q_CHECK_PTR(mw);

    QToolBar *testbar = new QToolBar("testbar");
    Q_CHECK_PTR(testbar);
    testbar->setObjectName("testbar");

    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QToolButton *btn_test = add_button(testbar,
                                       new QToolButton(this),
                                       qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                       "test",
                                       "test");
    
    connect(btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    emit info("Test");
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::read_data(QByteArray ba)
{
    emit info(ba.data());
}
//--------------------------------------------------------------------------------
void MainBox::f_move_up(uint8_t speed)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 8;
    packet.body.data1    = 0;
    packet.body.data2    = speed;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_move_down(uint8_t speed)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 0x10;
    packet.body.data1    = 0;
    packet.body.data2    = speed;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_move_left(uint8_t speed)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 4;
    packet.body.data1    = speed;
    packet.body.data2    = 0;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    //ba.append((char *)&packet.buf, sizeof(packet)); //TODO
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_move_right(uint8_t speed)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 2;
    packet.body.data1    = speed;
    packet.body.data2    = 0;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_move_up_left(uint8_t speed)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 0x0C;
    packet.body.data1    = speed;
    packet.body.data2    = speed;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_move_up_right(uint8_t speed)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 0x0A;
    packet.body.data1    = speed;
    packet.body.data2    = speed;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_move_down_left(uint8_t speed)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 0x14;
    packet.body.data1    = speed;
    packet.body.data2    = speed;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_move_down_right(uint8_t speed)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 0x12;
    packet.body.data1    = speed;
    packet.body.data2    = speed;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_zoom_in(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 0x20;
    packet.body.data1    = 0;
    packet.body.data2    = 0;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_zoom_out(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 0x40;
    packet.body.data1    = 0;
    packet.body.data2    = 0;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_move_stop(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = 0;
    packet.body.cmd2     = 0;
    packet.body.data1    = 0;
    packet.body.data2    = 0;
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::f_send(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = static_cast<uint8_t>(ui->sb_cmd1->value());
    packet.body.cmd2     = static_cast<uint8_t>(ui->sb_cmd2->value());
    packet.body.data1    = static_cast<uint8_t>(ui->sb_data1->value());
    packet.body.data2    = static_cast<uint8_t>(ui->sb_data2->value());
    packet.body.crc      = CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5);

    QByteArray ba;
    ba.append(reinterpret_cast<char *>(&packet.buf), sizeof(packet));

    emit send(ba);
    emit debug(QString("%1: send [%2]")
               .arg(Q_FUNC_INFO)
               .arg(ba.toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::refresh(void)
{
    PELCO_PACKET packet;
    packet.body.sync     = SYNC_BYTE;
    packet.body.address  = static_cast<uint8_t>(ui->sb_address->value());
    packet.body.cmd1     = static_cast<uint8_t>(ui->sb_cmd1->value());
    packet.body.cmd2     = static_cast<uint8_t>(ui->sb_cmd2->value());
    packet.body.data1    = static_cast<uint8_t>(ui->sb_data1->value());
    packet.body.data2    = static_cast<uint8_t>(ui->sb_data2->value());

    ui->sb_chk_sum->setValue(CRC::pelco_crc8(reinterpret_cast<uint8_t *>(&packet.buf[1]), 5));
}
//--------------------------------------------------------------------------------
void MainBox::pressed(void)
{
    QToolButton *btn = reinterpret_cast<QToolButton *>(sender());
    if(btn)
    {
        QString str = btn->property("button").toString();
        int speed = ui->sl_speed->value();
        emit debug(QString("%1 pressed").arg(str));

        if(str == "up_left")    f_move_up_left(static_cast<uint8_t>(speed));
        if(str == "up")         f_move_up(static_cast<uint8_t>(speed));
        if(str == "up_right")   f_move_up_right(static_cast<uint8_t>(speed));

        if(str == "left")       f_move_left(static_cast<uint8_t>(speed));
        if(str == "right")      f_move_right(static_cast<uint8_t>(speed));

        if(str == "down_left")  f_move_down_left(static_cast<uint8_t>(speed));
        if(str == "down")       f_move_down(static_cast<uint8_t>(speed));
        if(str == "down_right") f_move_down_right(static_cast<uint8_t>(speed));

        if(str == "zoom_in")    f_zoom_in();
        if(str == "zoom_out")   f_zoom_out();
    }
}
//--------------------------------------------------------------------------------
void MainBox::f_video(void)
{
    emit trace(Q_FUNC_INFO);
    if(ui->cb_address->currentText().isEmpty())
    {
        emit error("Address is emnpty!");
        return;
    }

    emit info("Run");
    const QUrl url1 = QUrl(ui->cb_address->currentText());
    //const QUrl url1 = QUrl("rtsp://192.168.0.66/av0_0");
    //const QUrl url1 = QUrl("rtsp://192.168.1.88/HD");

    const QNetworkRequest requestRtsp1(url1);
    player->setMedia(requestRtsp1);
    player->play();
}
//--------------------------------------------------------------------------------
void MainBox::f_error(QMediaPlayer::Error err)
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
void MainBox::released(void)
{
    QToolButton *btn = reinterpret_cast<QToolButton *>(sender());
    if(btn)
    {
        emit debug(QString("%1 released").arg(btn->property("button").toString()));
        f_move_stop();
    }
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
