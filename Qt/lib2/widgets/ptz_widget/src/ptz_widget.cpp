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
#include "ui_ptz_widget.h"
#include "defines.hpp"
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
    save_widgets("PTZ");
    if(settings)
    {
        settings->deleteLater();
    }
    if(player)
    {
        player->deleteLater();
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void PTZ_widget::create_player(void)
{
    player = new QMediaPlayer(this);
    player->setVideoOutput(ui->video_widget);

    connect(player, SIGNAL(error(QMediaPlayer::Error)), this,   SLOT(f_error(QMediaPlayer::Error)));
}
//--------------------------------------------------------------------------------
void PTZ_widget::create_tcp_socket(void)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,  SIGNAL(readyRead()),    this,   SLOT(read_data()));
    connect(tcpSocket,  SIGNAL(error(QAbstractSocket::SocketError)), this, (SLOT(s_error(QAbstractSocket::SocketError))));
    connect(tcpSocket,  SIGNAL(connected()),    this,   SLOT(f_connected()));
    connect(tcpSocket,  SIGNAL(disconnected()), this,   SLOT(f_disconnected()));
}
//--------------------------------------------------------------------------------
void PTZ_widget::init(void)
{
    ui->setupUi(this);

#ifndef SAVE_INI
    settings = new QSettings(ORGNAME, APPNAME);
#else
    settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
#endif

    create_tcp_socket();
    create_player();
    create_position_widgets();

    ui->video_widget->setMinimumSize(640, 480);

    ui->sl_speed->setRange(0, 100);
    ui->sl_speed->setValue(50);
    ui->sl_brightness->setRange(0, 255);
    ui->sl_contrast->setRange(0, 255);
    ui->sl_tone->setRange(0, 255);
    ui->sl_saturation->setRange(0, 255);
    ui->sl_iris->setRange(0, 13);
    ui->sl_shutter->setRange(0, 21);
    ui->sl_gamma->setRange(0, 4);
    ui->sl_sharpness->setRange(0, 15);
    ui->sl_noise->setRange(0, 5);

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

    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(f_test()));

    connect(ui->btn_choice, SIGNAL(clicked(bool)),  this,   SLOT(choice()));

    connect(ui->btn_wiper_on,   SIGNAL(clicked(bool)),  this,   SLOT(f_wiper_on()));
    connect(ui->btn_wiper_off,  SIGNAL(clicked(bool)),  this,   SLOT(f_wiper_off()));
    connect(ui->btn_light_on,   SIGNAL(clicked(bool)),  this,   SLOT(f_light_on()));
    connect(ui->btn_light_off,  SIGNAL(clicked(bool)),  this,   SLOT(f_light_off()));
    connect(ui->btn_diaphragm_plus,     SIGNAL(clicked(bool)),  this,   SLOT(f_diaphragm_plus()));
    connect(ui->btn_diaphragm_minus,    SIGNAL(clicked(bool)),  this,   SLOT(f_diaphragm_minus()));
    connect(ui->btn_zoom_plus,  SIGNAL(clicked(bool)),  this,   SLOT(f_zoom_plus()));
    connect(ui->btn_zoom_minus, SIGNAL(clicked(bool)),  this,   SLOT(f_zoom_minus()));

#if 1
    connect(ui->sl_brightness,  SIGNAL(sliderReleased()),  this,   SLOT(f_set_brightness()));
    connect(ui->sl_contrast,    SIGNAL(sliderReleased()),  this,   SLOT(f_set_contrast()));
    connect(ui->sl_tone,        SIGNAL(sliderReleased()),  this,   SLOT(f_set_tone()));
    connect(ui->sl_saturation,  SIGNAL(sliderReleased()),  this,   SLOT(f_set_saturation()));
    connect(ui->sl_iris,        SIGNAL(sliderReleased()),  this,   SLOT(f_set_iris()));
    connect(ui->sl_shutter,     SIGNAL(sliderReleased()),  this,   SLOT(f_set_shutter()));
    connect(ui->sl_gamma,       SIGNAL(sliderReleased()),  this,   SLOT(f_set_gamma()));
    connect(ui->sl_sharpness,   SIGNAL(sliderReleased()),  this,   SLOT(f_set_sharpness()));
    connect(ui->sl_noise,       SIGNAL(sliderReleased()),  this,   SLOT(f_set_noise()));
#else
    connect(ui->sl_brightness,  SIGNAL(valueChanged(int)),  this,   SLOT(f_set_brightness(int)));
    connect(ui->sl_contrast,    SIGNAL(valueChanged(int)),  this,   SLOT(f_set_contrast(int)));
    connect(ui->sl_tone,        SIGNAL(valueChanged(int)),  this,   SLOT(f_set_tone(int)));
    connect(ui->sl_saturation,  SIGNAL(valueChanged(int)),  this,   SLOT(f_set_saturation(int)));
    connect(ui->sl_iris,        SIGNAL(valueChanged(int)),  this,   SLOT(f_set_iris(int)));
    connect(ui->sl_shutter,     SIGNAL(valueChanged(int)),  this,   SLOT(f_set_shutter(int)));
    connect(ui->sl_gamma,       SIGNAL(valueChanged(int)),  this,   SLOT(f_set_gamma(int)));
    connect(ui->sl_sharpness,   SIGNAL(valueChanged(int)),  this,   SLOT(f_set_sharpness(int)));
    connect(ui->sl_noise,       SIGNAL(valueChanged(int)),  this,   SLOT(f_set_noise(int)));
#endif

    //---
    QList<QAbstractButton *> sl_buttons;
    sl_buttons.append(new QPushButton("+"));
    sl_buttons.append(new QPushButton("-"));

    QList<QAbstractButton *> sl_buttons2;
    sl_buttons2.append(new QPushButton("ON"));
    sl_buttons2.append(new QPushButton("OFF"));

    QList<QAbstractButton *> sl_buttons3;
    sl_buttons3.append(new QPushButton("ON"));
    sl_buttons3.append(new QPushButton("OFF"));

    add_buttons(0, "test",   sl_buttons);
    add_slider(1,  "slider", 1, 100);
    add_buttons(2, "WIPER",  sl_buttons2);
    add_buttons(3, "LIGHT",  sl_buttons3);
    add_slider(4,  "slider2", 1, 10);
    //---
    l_params.append({ "buttons", "ptz", "STOP", 0, 0 });
    //---

    load_widgets("PTZ");
}
//--------------------------------------------------------------------------------
void PTZ_widget::add_buttons(int index,
                             QString name,
                             QList<QAbstractButton *> buttons)
{
    QLabel *caption = new QLabel(this);
    caption->setText(name);

    ui->grid->addWidget(caption, index, 0);

    int x = 1;
    foreach (QAbstractButton *btn, buttons)
    {
        ui->grid->addWidget(btn, index, x);
        x++;
    }
}
//--------------------------------------------------------------------------------
void PTZ_widget::add_slider(int index,
                            QString name,
                            int min_value,
                            int max_value)
{
    QLabel *caption = new QLabel(this);
    caption->setText(name);

    ui->grid->addWidget(caption, index, 0);

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(min_value, max_value);
    slider->setObjectName(QString("sl_%1").arg(name));

    ui->grid->addWidget(slider, index, 1, 1, 2);
}
//--------------------------------------------------------------------------------
void PTZ_widget::create_position_widgets(void)
{
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
void PTZ_widget::s_error(QAbstractSocket::SocketError err)
{
    switch (err)
    {
    case QAbstractSocket::UnconnectedState: emit error("UnconnectedState"); break;
    case QAbstractSocket::HostLookupState:  emit error("HostLookupState");  break;
    case QAbstractSocket::ConnectingState:  emit error("ConnectingState");  break;
    case QAbstractSocket::ConnectedState:   emit error("ConnectedState");   break;
    case QAbstractSocket::BoundState:       emit error("BoundState");       break;
    case QAbstractSocket::ListeningState:   emit error("ListeningState");   break;
    case QAbstractSocket::ClosingState:     emit error("ClosingState");     break;

    default:
        emit error(QString("unknown error %1").arg(err));
        break;
    }
}
//--------------------------------------------------------------------------------
void PTZ_widget::read_data(void)
{

}
//--------------------------------------------------------------------------------
void PTZ_widget::f_connected(void)
{
    emit info("Connected");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_disconnected(void)
{
    emit info("Disconnected");
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
    send_cmd("ptz", "STOP", 0, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left_up(void)
{
    send_cmd("ptz", "LU", ui->sl_speed->value(), ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right_up(void)
{
    send_cmd("ptz", "RU", ui->sl_speed->value(), ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left(void)
{
    send_cmd("ptz", "L", ui->sl_speed->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right(void)
{
    send_cmd("ptz", "R", ui->sl_speed->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_up(void)
{
    send_cmd("ptz", "U", 0, ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_down(void)
{
    send_cmd("ptz", "D", 0, ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left_down(void)
{
    send_cmd("ptz", "LD", ui->sl_speed->value(), ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right_down(void)
{
    send_cmd("ptz", "RD", ui->sl_speed->value(), ui->sl_speed->value());
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_test(void)
{
    emit info("test");

    //send_cmd("ptz", "AD", "SA", "WIPER");
    //send_cmd("ptz", "AD", "CA", "WIPER");

    //send_cmd("ptz", "AD", "SA", "LIGHT");
    //send_cmd("ptz", "AD", "CA", "LIGHT");

    //send_cmd("ptz", "O", 0, 0);
    //send_cmd("ptz", "D", 0, 0);

    //send_cmd("isp", "contrast", 128, 0);

    //send_cmd("isp", "mirror", 0, 0);
    //send_cmd("isp", "mirror", 1, 0);

    //send_cmd("isp", "flip", 0, 0);
    send_cmd("isp", "flip", 1, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_brightness(void)
{
    send_cmd("isp", "brightness", ui->sl_brightness->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_contrast(void)
{
    send_cmd("isp", "contrast", ui->sl_contrast->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_tone(void)
{
    send_cmd("isp", "tone", ui->sl_tone->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_saturation(void)
{
    send_cmd("isp", "saturation", ui->sl_saturation->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_iris(void)
{
    send_cmd("isp", "iris", ui->sl_iris->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_shutter(void)
{
    send_cmd("isp", "shutter", ui->sl_shutter->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_gamma(void)
{
    send_cmd("isp", "gamma", ui->sl_gamma->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_sharpness(void)
{
    send_cmd("isp", "sharpness", ui->sl_sharpness->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_noise(void)
{
    send_cmd("isp", "noise", ui->sl_noise->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_brightness(int value)
{
    send_cmd("isp", "brightness", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_contrast(int value)
{
    send_cmd("isp", "contrast", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_tone(int value)
{
    send_cmd("isp", "tone", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_saturation(int value)
{
    send_cmd("isp", "saturation", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_iris(int value)
{
    send_cmd("isp", "iris", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_shutter(int value)
{
    send_cmd("isp", "shutter", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_gamma(int value)
{
    send_cmd("isp", "gamma", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_sharpness(int value)
{
    send_cmd("isp", "sharpness", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_set_noise(int value)
{
    send_cmd("isp", "noise", value, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_wiper_on(void)
{
    send_cmd("ptz", "AD", "SA", "WIPER");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_wiper_off(void)
{
    send_cmd("ptz", "AD", "CA", "WIPER");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_light_on(void)
{
    send_cmd("ptz", "AD", "SA", "LIGHT");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_light_off(void)
{
    send_cmd("ptz", "AD", "CA", "LIGHT");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_diaphragm_plus(void)
{
    send_cmd("ptz", "O", 0, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_diaphragm_minus(void)
{
    send_cmd("ptz", "D", 0, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_zoom_plus(void)
{
    send_cmd("ptz", "T", 0, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_zoom_minus(void)
{
    send_cmd("ptz", "W", 0, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::send_cmd(QString cmd,
                          QString func,
                          QVariant param_1,
                          QVariant param_2)
{
    emit trace(Q_FUNC_INFO);

    QString param;
    param.append(QString("http://%1/cgi-bin/senddata.cgi?").arg(url.host()));
    param.append(QString("cmd=%1;").arg(cmd));
    param.append(QString("func=%1;").arg(func));

    if(param_1.type() == QVariant::Int)
    {
        param.append(QString("value1=%1;").arg(param_1.toInt()));
    }
    if(param_1.type() == QVariant::String)
    {
        param.append(QString("value1=%1;").arg(param_1.toString()));
    }

    if(param_2.type() == QVariant::Int)
    {
        param.append(QString("value2=%1;").arg(param_2.toInt()));
    }
    if(param_2.type() == QVariant::String)
    {
        param.append(QString("value2=%1;").arg(param_2.toString()));
    }

    bool ok = false;
    ok = tcpSocket->isOpen();
    if(ok == false)
    {
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
        f_disconnect();
        return;
    }
    ok = tcpSocket->waitForBytesWritten(1000);
    if(!ok)
    {
        emit error("waitForBytesWritten");
        f_disconnect();
        return;
    }
#if 0
    ok = tcpSocket->waitForReadyRead(1000);
    if(!ok)
    {
        emit error("waitForReadyRead");
        f_disconnect();
        return;
    }
#endif
    f_disconnect();

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
void PTZ_widget::f_disconnect(void)
{
    if(tcpSocket->isOpen())
    {
        tcpSocket->close();
    }
    tcpSocket->disconnectFromHost();
}
//--------------------------------------------------------------------------------
void PTZ_widget::load_widgets(QString group_name)
{
    QList<QSlider *> allobj = findChildren<QSlider *>();
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach (QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setSliderPosition(settings->value("position", 0).toDouble());
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void PTZ_widget::save_widgets(QString group_name)
{
    QList<QSlider *> allobj = findChildren<QSlider *>();
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    foreach(QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("position", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();
}
//--------------------------------------------------------------------------------
void PTZ_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
