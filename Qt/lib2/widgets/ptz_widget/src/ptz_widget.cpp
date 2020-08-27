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
    MyWidget(parent),
    ui(new Ui::PTZ_widget)
{
    init();
}
//--------------------------------------------------------------------------------
PTZ_widget::~PTZ_widget()
{
    save_string(IP_STRING, ui->ip_widget->get_url().host());
    save_int(PORT_STRING, ui->ip_widget->get_url().port());

    save_widgets();
    if(player)
    {
        delete player;
    }
    delete ui;
}
//--------------------------------------------------------------------------------
void PTZ_widget::create_player(void)
{
    player = new QMediaPlayer(this);
    player->setVolume(0);   //TODO выключить звук
    player->setVideoOutput(ui->video_widget);

    ui->le_proto->setText("rtsp");
    ui->le_proto->setProperty(NO_SAVE, true);
    ui->le_proto->setReadOnly(true);

    connect_position_widgets();

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

    create_tcp_socket();
    create_player();

    ui->video_widget->setMinimumSize(640, 480);

    ui->sl_speed->setRange(0, 100);
    ui->sl_speed->setValue(50);

    ui->sb_port->setRange(0, 0xFFFF);

    ui->le_login->setText("admin");
    ui->le_password->setText("admin");
    ui->sb_port->setValue(81);
#ifdef Q_OS_LINUX
    ui->ip_widget->set_url(QUrl("ip://192.168.1.12"));
#else
    ui->ip_widget->set_url(QUrl("ip://192.168.1.101"));
#endif

    ui->le_proto->setEnabled(false);
    ui->le_login->setEnabled(false);
    ui->le_password->setEnabled(false);
    ui->ip_widget->setEnabled(false);
    ui->sb_port->setEnabled(false);
    ui->le_param->setEnabled(false);

    ui->btn_play->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btn_stop->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));

    connect(ui->btn_play,   SIGNAL(clicked(bool)),  this,   SLOT(play()));
    connect(ui->btn_stop,   SIGNAL(clicked(bool)),  this,   SLOT(stop()));

    connect(ui->btn_test,   SIGNAL(clicked(bool)),  this,   SLOT(f_test()));

    connect(ui->btn_choice, SIGNAL(clicked(bool)),  this,   SLOT(choice()));

    //---
    int index = 0;
    PTZ_PARAM param;
    PTZ_PARAM param1;
    PTZ_PARAM param2;

    param1.min_value = 0;
    param1.max_value = 0;
    param2.min_value = 0;
    param2.max_value = 0;

    param.cmd = "isp";
    param.func = "brightness";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "brightness", param);

    param.cmd = "isp";
    param.func = "contrast";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "contrast", param);

    param.cmd = "isp";
    param.func = "iris";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "iris", param);

    param.cmd = "isp";
    param.func = "gamma";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "gamma", param);

    param.cmd = "isp";
    param.func = "noise";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "noise", param);

    param.cmd = "isp";
    param.func = "tone";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "tone", param);

    param.cmd = "isp";
    param.func = "saturation";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "saturation", param);

    param.cmd = "isp";
    param.func = "shutter";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "shutter", param);

    param.cmd = "isp";
    param.func = "sharpness";
    param.min_value = 1;
    param.max_value = 100;
    add_slider(index++, "sharpness", param);

    param1.cmd = "ptz";
    param1.func = "AD";
    param1.param1 = "SA";
    param1.param2 = "WIPER";
    param1.btn_caption = "ON";
    param2.cmd = "ptz";
    param2.func = "AD";
    param2.param1 = "CA";
    param1.param2 = "WIPER";
    param2.btn_caption = "OFF";
    add_buttons(index++, "WIPER", param1, param2);

    param1.cmd = "ptz";
    param1.func = "AD";
    param1.param1 = "SA";
    param1.param2 = "LIGHT";
    param1.btn_caption = "ON";
    param2.cmd = "ptz";
    param2.func = "AD";
    param2.param1 = "CA";
    param1.param2 = "LIGHT";
    param2.btn_caption = "OFF";
    add_buttons(index++, "LIGHT", param1, param2);

    param1.cmd = "ptz";
    param1.func = "AD";
    param1.param1 = "SA";
    param1.param2 = "DIAPHGRAM";
    param1.btn_caption = "+";
    param2.cmd = "ptz";
    param2.func = "AD";
    param2.param1 = "CA";
    param1.param2 = "DIAPHGRAM";
    param2.btn_caption = "-";
    add_buttons(index++, "DIAPHGRAM", param1, param2);

    param1.cmd = "ptz";
    param1.func = "T";
    param1.param1 = 0;
    param1.param2 = 0;
    param1.btn_caption = "+";
    param2.cmd = "ptz";
    param2.func = "W";
    param2.param1 = 0;
    param1.param2 = 0;
    param2.btn_caption = "-";
    add_buttons(index++, "ZOOM", param1, param2);

    param1.cmd = "isp";
    param1.func = "mirror";
    param1.param1 = 1;
    param1.param2 = 0;
    param1.btn_caption = "ON";
    param2.cmd = "isp";
    param2.func = "mirror";
    param2.param1 = 0;
    param1.param2 = 0;
    param2.btn_caption = "OFF";
    add_buttons(index++, "MIRROR", param1, param2);

    param1.cmd = "isp";
    param1.func = "flip";
    param1.param1 = 1;
    param1.param2 = 0;
    param1.btn_caption = "ON";
    param2.cmd = "isp";
    param2.func = "flip";
    param2.param1 = 0;
    param1.param2 = 0;
    param2.btn_caption = "OFF";
    add_buttons(index++, "FLIP", param1, param2);
    //---

    url.setHost(ui->ip_widget->get_url().host());
    url.setPort(ui->sb_port->value());

    url.setHost(load_string(IP_STRING));
    int value = 0;
    load_int(PORT_STRING, &value);
    url.setPort(value);

    ui->ip_widget->set_url(url);

    lock_widgets();

    load_widgets();

    //play();
}
//--------------------------------------------------------------------------------
void PTZ_widget::widgets_set_state(bool state)
{
    ui->btn_u->setEnabled(state);
    ui->btn_d->setEnabled(state);
    ui->btn_l->setEnabled(state);
    ui->btn_r->setEnabled(state);
    ui->btn_lu->setEnabled(state);
    ui->btn_ld->setEnabled(state);
    ui->btn_ru->setEnabled(state);
    ui->btn_rd->setEnabled(state);
    ui->btn_lu->setEnabled(state);
    ui->sl_speed->setEnabled(state);

    QList<QPushButton *> all_btn = findChildren<QPushButton *>();
    foreach(QPushButton *btn, all_btn)
    {
        if(btn->property("cmd").toString().isEmpty() == false)
        {
            btn->setEnabled(state);
        }
    }

    QList<QSlider *> all_slider = findChildren<QSlider *>();
    foreach(QSlider *slider, all_slider)
    {
        if(slider->property("cmd").toString().isEmpty() == false)
        {
            slider->setEnabled(state);
        }
    }
}
//--------------------------------------------------------------------------------
void PTZ_widget::lock_widgets(void)
{
    widgets_set_state(false);
}
//--------------------------------------------------------------------------------
void PTZ_widget::unlock_widgets(void)
{
    widgets_set_state(true);
}
//--------------------------------------------------------------------------------
void PTZ_widget::add_buttons(int index,
                             QString name,
                             PTZ_PARAM param1,
                             PTZ_PARAM param2)
{
    QLabel *caption = new QLabel(this);
    caption->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    caption->setText(name);

    ui->grid->addWidget(caption, index, 0);

    QPushButton *btn1 = new QPushButton(this);
    btn1->setObjectName(QString("btn1_%1").arg(name));
    btn1->setText(param1.btn_caption);
    btn1->setProperty("cmd",      param1.cmd);
    btn1->setProperty("func",     param1.func);
    btn1->setProperty("param1",   param1.param1);
    btn1->setProperty("param2",   param1.param2);

    QPushButton *btn2 = new QPushButton(this);
    btn2->setObjectName(QString("btn2_%1").arg(name));
    btn2->setText(param2.btn_caption);
    btn2->setProperty("cmd",      param2.cmd);
    btn2->setProperty("func",     param2.func);
    btn2->setProperty("param1",   param2.param1);
    btn2->setProperty("param2",   param2.param2);

    connect(btn1,   SIGNAL(clicked(bool)),  this,   SLOT(f_push_button()));
    connect(btn2,   SIGNAL(clicked(bool)),  this,   SLOT(f_push_button()));

    ui->grid->addWidget(btn1, index, 1);
    ui->grid->addWidget(btn2, index, 2);
}
//--------------------------------------------------------------------------------
void PTZ_widget::add_slider(int index,
                            QString name,
                            PTZ_PARAM params)
{
    QLabel *caption = new QLabel(this);
    caption->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    caption->setText(name);

    ui->grid->addWidget(caption, index, 0);

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setObjectName(QString("sl_%1").arg(name));
    slider->setRange(params.min_value,
                     params.max_value);
    slider->setProperty("cmd",      params.cmd);
    slider->setProperty("func",     params.func);
    slider->setProperty("param1",   params.param1);
    slider->setProperty("param2",   params.param2);

    connect(slider, SIGNAL(sliderReleased()),   this,   SLOT(f_move_slider()));

    ui->grid->addWidget(slider, index, 1, 1, 2);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_push_button(void)
{
    QPushButton *btn = reinterpret_cast<QPushButton *>(sender());
    if(btn == nullptr)
    {
        return;
    }
    QString cmd = btn->property("cmd").toString();
    QString func = btn->property("func").toString();
    QVariant param1 = btn->property("param1").toString();
    QVariant param2 = btn->property("param2").toString();

    emit debug(QString("%1").arg(btn->objectName()));

    emit debug(QString("cmd [%1]").arg(cmd));
    emit debug(QString("func [%1]").arg(func));
    emit debug(QString("param1 [%1]").arg(param1.toString()));
    emit debug(QString("param2 [%1]").arg(param2.toString()));

    send_cmd(cmd, func, param1, param2);
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_move_slider(void)
{
    QSlider *slider = reinterpret_cast<QSlider *>(sender());
    if(slider == nullptr)
    {
        return;
    }
    QString  cmd = slider->property("cmd").toString();
    QString  func = slider->property("func").toString();
    //QVariant param1 = slider->property("param1").toString();
    //QVariant param2 = slider->property("param2").toString();

    emit debug(QString("%1").arg(slider->objectName()));

    emit debug(QString("cmd [%1]").arg(cmd));
    emit debug(QString("func [%1]").arg(func));
    //emit debug(QString("param1 [%1]").arg(param1.toString()));
    //emit debug(QString("param2 [%1]").arg(param2.toString()));

    //send_cmd(cmd, func, param1, param2);
    send_cmd(cmd, func, slider->value(), 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::connect_position_widgets(void)
{
    connect(ui->btn_lu, SIGNAL(pressed()),  this,   SLOT(f_left_up()));
    connect(ui->btn_ru, SIGNAL(pressed()),  this,   SLOT(f_right_up()));
    connect(ui->btn_l,  SIGNAL(pressed()),  this,   SLOT(f_left()));
    connect(ui->btn_r,  SIGNAL(pressed()),  this,   SLOT(f_right()));
    connect(ui->btn_u,  SIGNAL(pressed()),  this,   SLOT(f_up()));
    connect(ui->btn_d,  SIGNAL(pressed()),  this,   SLOT(f_down()));
    connect(ui->btn_ld, SIGNAL(pressed()),  this,   SLOT(f_left_down()));
    connect(ui->btn_rd, SIGNAL(pressed()),  this,   SLOT(f_right_down()));

    connect(ui->btn_lu, SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_ru, SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_l,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_r,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_u,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_d,  SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_ld, SIGNAL(released()), this,   SLOT(f_stop()));
    connect(ui->btn_rd, SIGNAL(released()), this,   SLOT(f_stop()));
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_error(QMediaPlayer::Error err)
{
    if(err != QMediaPlayer::NoError)
    {
        lock_widgets();
    }
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
void PTZ_widget::s_state(QAbstractSocket::SocketState state)
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
void PTZ_widget::s_error(QAbstractSocket::SocketError err)
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
QString PTZ_widget::get_full_url(void)
{
    QString temp = QString("rtsp://%1:%2@%3:%4/%5")
            .arg(ui->le_login->text())
            .arg(ui->le_password->text())
            .arg(ui->ip_widget->get_url().host())
            .arg(ui->sb_port->value())
            .arg(ui->le_param->text());
    return temp;
}
//--------------------------------------------------------------------------------
void PTZ_widget::play(void)
{
    if(player->isAvailable())
    {
        player->stop();

        url = QUrl(get_full_url());
        const QNetworkRequest requestRtsp(url);
        player->setMedia(requestRtsp);
        player->play();

        unlock_widgets();
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
        lock_widgets();
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
    dlg->set_IP(ui->ip_widget->get_url());
    dlg->set_login(ui->le_login->text());
    dlg->set_password(ui->le_password->text());
    dlg->set_port(ui->sb_port->value());
    dlg->set_param(ui->le_param->text());

    int btn = dlg->exec();
    if(btn == PTZ_dialog::Accepted)
    {
        ui->le_login->setText(dlg->get_login());
        ui->le_password->setText(dlg->get_password());
        ui->ip_widget->set_url(dlg->get_IP());
        ui->sb_port->setValue(dlg->get_port());
        ui->le_login->setText(dlg->get_login());

        QByteArray ba;
        ba.append(ui->ip_widget->get_url().host());
        url = QUrl::fromEncoded(ba);
        url.setPort(port);
    }
    dlg->deleteLater();
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_stop(void)
{
    //send_cmd("ptz", "STOP", 0, 0);
    send_cmd("stop");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left_up(void)
{
    //send_cmd("ptz", "LU", ui->sl_speed->value(), ui->sl_speed->value());
    send_cmd("leftup");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right_up(void)
{
    //send_cmd("ptz", "RU", ui->sl_speed->value(), ui->sl_speed->value());
    send_cmd("rightup");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left(void)
{
    //send_cmd("ptz", "L", ui->sl_speed->value(), 0);
    send_cmd("left");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right(void)
{
    //send_cmd("ptz", "R", ui->sl_speed->value(), 0);
    send_cmd("right");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_up(void)
{
    //send_cmd("ptz", "U", 0, ui->sl_speed->value());
    send_cmd("up");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_down(void)
{
    //send_cmd("ptz", "D", 0, ui->sl_speed->value());
    send_cmd("down");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_left_down(void)
{
    //send_cmd("ptz", "LD", ui->sl_speed->value(), ui->sl_speed->value());
    send_cmd("leftdown");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_right_down(void)
{
    //send_cmd("ptz", "RD", ui->sl_speed->value(), ui->sl_speed->value());
    send_cmd("rightdown");
}
//--------------------------------------------------------------------------------
void PTZ_widget::f_test(void)
{
    emit info("test");

    emit info(QString("margin %1").arg(ui->grid->margin()));
    emit info(QString("spacing %1").arg(ui->grid->spacing()));

    //send_cmd("isp", "flip", 0, 0);
    //send_cmd("isp", "flip", 1, 0);
}
//--------------------------------------------------------------------------------
void PTZ_widget::send_cmd(QString  cmd)
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
void PTZ_widget::send_cmd(QString  cmd,
                          QString  func,
                          QVariant param_1,
                          QVariant param_2)
{
    emit trace(Q_FUNC_INFO);

    QString param;

    //param.append(QString("https://admin:admin@%1:81/cgi-bin/senddata.cgi?").arg(url.host()));

    //param.append(QString("http://%1/cgi-bin/senddata.cgi?").arg(url.host()));
    param.append("http://");
    param.append("admin:admin@");
    param.append(url.host());
    param.append(QString(":%1").arg(url.port()));
    param.append("/cgi-bin/senddata.cgi?");

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

    emit trace(QString("param = %1").arg(param));

    bool ok = false;
    ok = tcpSocket->isOpen();
    if(ok == false)
    {
        ok = f_connect();
        if(ok == false)
        {
            emit error("connect fail");
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

    emit debug(QString("f_connect: %1:%2")
               .arg(ip)
               .arg(port));

    tcpSocket->connectToHost(ip, static_cast<quint16>(port));
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
void PTZ_widget::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool PTZ_widget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void PTZ_widget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void PTZ_widget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
