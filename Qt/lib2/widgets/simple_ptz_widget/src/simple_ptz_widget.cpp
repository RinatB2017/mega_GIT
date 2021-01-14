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
#include "myfiledialog.hpp"
#include "ui_simple_ptz_widget.h"
#include "mainwindow.hpp"
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
    Q_ASSERT(player);

    player->stop();
    disconnect(player, static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
               this,   &Simple_PTZ_widget::f_error);
    delete player;

    Q_ASSERT(probe);

    disconnect(probe,  &QVideoProbe::videoFrameProbed,
               this,   &Simple_PTZ_widget::processFrame);
    delete probe;

    Q_ASSERT(networkManager);

    disconnect(networkManager,     &QNetworkAccessManager::finished,
               this,               &Simple_PTZ_widget::onFinished);

    delete networkManager;
    delete ui;
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::init(void)
{
    ui->setupUi(this);

    //ui->video_widget->setMinimumSize(M_WIDTH, M_HEIGHT);
    ui->btn_screenshot->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogSaveButton));

    ui->btn_other_cmd->setIcon(qApp->style()->standardIcon(QStyle::SP_CommandLink));

    networkManager = new QNetworkAccessManager(this);

    connect(ui->btn_connect,    &QPushButton::clicked,
            this,               &Simple_PTZ_widget::play);
    connect(ui->btn_disconnect, &QPushButton::clicked,
            this,               &Simple_PTZ_widget::stop);

    connect(networkManager,     &QNetworkAccessManager::finished,
            this,               &Simple_PTZ_widget::onFinished);

    connect(ui->ipv4_widget,    &IPV4::editingFinished,
            this,               &Simple_PTZ_widget::play);
    connect(ui->le_login,       &QLineEdit::editingFinished,
            this,               &Simple_PTZ_widget::play);
    connect(ui->le_password,    &QLineEdit::editingFinished,
            this,               &Simple_PTZ_widget::play);

    connect(ui->btn_screenshot, &QPushButton::clicked,
            this,               &Simple_PTZ_widget::f_screenshot);

    connect(ui->btn_other_cmd,  &QPushButton::clicked,
            this,               &Simple_PTZ_widget::f_other_cmd);

    connect(ui->le_other_cmd,   &QLineEdit::returnPressed,
            this,               &Simple_PTZ_widget::f_other_cmd);

    connect(ui->btn_test,       &QPushButton::clicked,
            this,               &Simple_PTZ_widget::f_test);

    create_player();
    connect_position_widgets();

#ifdef USE_COMMUNICATIONS
    create_socket();
#endif

    ui->btn_d->setDisabled(true);
    ui->btn_l->setDisabled(true);
    ui->btn_r->setDisabled(true);
    ui->btn_u->setDisabled(true);
    ui->btn_up_down->setDisabled(true);
    ui->btn_left_right->setDisabled(true);

#ifndef USE_COMMUNICATIONS
    ui->communications_frame->setVisible(false);
#endif

#ifndef QT_DEBUG
    ui->le_other_cmd->setVisible(false);
    ui->btn_other_cmd->setVisible(false);
#endif

#ifndef QT_DEBUG
    ui->btn_test->setVisible(false);
#endif

#if 0
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
        mw->add_dock_widget("commands", "commands", Qt::LeftDockWidgetArea,     ui->command_frame);
        mw->add_dock_widget("rtsp",     "rtsp",     Qt::RightDockWidgetArea,    ui->ptz_frame);
        setVisible(false);
    }
#endif
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::create_player(void)
{
    // player = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    player = new QMediaPlayer(this);
    player->setVolume(0);   //TODO выключить звук
    player->setVideoOutput(ui->video_widget);

    connect(player, static_cast<void (QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
            this,   &Simple_PTZ_widget::f_error);

    probe = new QVideoProbe(this);
    connect(probe,  &QVideoProbe::videoFrameProbed,
            this,   &Simple_PTZ_widget::processFrame);
    probe->setSource(player); // Returns true, hopefully.
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::processFrame(const QVideoFrame &frame)
{
    if(player->state() == QMediaPlayer::PlayingState)
    {
        current_frame = frame;
        emit get_frame(frame);

        ui->mediarecorder_widget->get_frame(frame);

#ifdef USE_COMMUNICATIONS
        if(socket->isOpen())
        {
            if(frame.isValid())
            {
                // https://coderoad.ru/37724602/%D0%9A%D0%B0%D0%BA-%D1%81%D0%BE%D1%85%D1%80%D0%B0%D0%BD%D0%B8%D1%82%D1%8C-%D0%BA%D0%B0%D0%B4%D1%80-%D1%81-%D0%BF%D0%BE%D0%BC%D0%BE%D1%89%D1%8C%D1%8E-QMediaPlayer
                QVideoFrame cloneFrame(frame);
                bool ok = cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
                if(!ok)
                {
                    emit error("Error create map");
                    return;
                }

                QByteArray block;
                block.append(reinterpret_cast<const char *>(cloneFrame.bits()), cloneFrame.mappedBytes());
                emit info(QString("send %1 bytes").arg(block.length()));
                socket->write(block);

                cloneFrame.unmap();
            }
            else
            {
                emit error("Frame not valid!");
                return;
            }
        }
#endif
    }
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::connect_position_widgets(void)
{
    connect(ui->btn_l,  &QToolButton::pressed,  this,   &Simple_PTZ_widget::f_left);
    connect(ui->btn_r,  &QToolButton::pressed,  this,   &Simple_PTZ_widget::f_right);
    connect(ui->btn_u,  &QToolButton::pressed,  this,   &Simple_PTZ_widget::f_up);
    connect(ui->btn_d,  &QToolButton::pressed,  this,   &Simple_PTZ_widget::f_down);

    connect(ui->btn_l,  &QToolButton::released, this,   &Simple_PTZ_widget::f_stop);
    connect(ui->btn_r,  &QToolButton::released, this,   &Simple_PTZ_widget::f_stop);
    connect(ui->btn_u,  &QToolButton::released, this,   &Simple_PTZ_widget::f_stop);
    connect(ui->btn_d,  &QToolButton::released, this,   &Simple_PTZ_widget::f_stop);

    connect(ui->btn_left_right, &QPushButton::clicked,  this,   &Simple_PTZ_widget::f_left_right);
    connect(ui->btn_up_down,    &QPushButton::clicked,  this,   &Simple_PTZ_widget::f_up_down);

    connect(ui->btn_stop,   &QToolButton::clicked,      this,   &Simple_PTZ_widget::f_stop);
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
        qDebug() << data;
    }
    else
    {
        emit error(reply->errorString());
    }
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::play(void)
{
    emit trace(Q_FUNC_INFO);

    if(player->isAvailable())
    {
        player->stop();

        QString url_str;
        url_str.append("rtsp://");
        url_str.append(ui->le_login->text());
        url_str.append(":");
        url_str.append(ui->le_password->text());
        url_str.append("@");
        url_str.append(ui->ipv4_widget->get_url().host());
        url_str.append(":");
        url_str.append(QString("%1").arg(ui->ipv4_widget->get_url().port()));

        emit debug(url_str);

        const QNetworkRequest requestRtsp(url_str);
        player->setMedia(requestRtsp);
        player->setVolume(0);   //TODO установить громкость
        player->play();

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
void Simple_PTZ_widget::f_test(void)
{
    emit info("Test");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::pause(void)
{
    emit trace(Q_FUNC_INFO);

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
    emit trace(Q_FUNC_INFO);

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
void Simple_PTZ_widget::f_other_cmd(void)
{
    send_other_cmd(ui->le_other_cmd->text());
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::f_screenshot(void)
{
    QImage img = current_frame.image();

    MyFileDialog *dlg = new MyFileDialog("screenshot", "screenshot");
    dlg->setAcceptMode(MyFileDialog::AcceptSave);
    dlg->setNameFilter("PNG files (*.png)");
    dlg->setDefaultSuffix("png");
    dlg->setDirectory(".");
    dlg->selectFile("screenshot");
    dlg->setOption(MyFileDialog::DontConfirmOverwrite, false);
    // dlg->setConfirmOverwrite(true);
    if(dlg->exec())
    {
        QStringList files = dlg->selectedFiles();
        QString filename = files.at(0);
        bool ok = img.save(filename);
        if(ok)
        {
            emit info(QString("File %1 created").arg(filename));
        }
        else
        {
            emit error(QString("File %1 NOT created").arg(filename));
        }
    }
    delete dlg;
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::send_cmd(const QString &cmd)
{
    emit trace(Q_FUNC_INFO);

    QString param;
    param.append(QString("http://%1:%2/moveptz.xml?dir=%3")
                 .arg(ui->ipv4_widget->get_url().host())
                 .arg(ui->ipv4_widget->get_url().port())
                 .arg(cmd));

    QString concatenated = ui->le_login->text() + ":" + ui->le_password->text(); //username:password
    //emit debug(QString("%1").arg(concatenated));

    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    QNetworkRequest request=QNetworkRequest(QUrl( param ));
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
    request.setRawHeader("Connection:", "keep-alive");
    networkManager->get(request);

    emit debug(param);
    // emit info("OK");
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::send_other_cmd(const QString &cmd)
{
    emit trace(Q_FUNC_INFO);

    QString param;
    param.append(QString("http://%1:%2/%3")
                 .arg(ui->ipv4_widget->get_url().host())
                 .arg(ui->ipv4_widget->get_url().port())
                 .arg(cmd));

    QString concatenated = ui->le_login->text() + ":" + ui->le_password->text(); //username:password
    //emit debug(QString("%1").arg(concatenated));

    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    QNetworkRequest request=QNetworkRequest(QUrl( param ));
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
    request.setRawHeader("Connection:", "keep-alive");
    networkManager->get(request);

    emit debug(param);
    // emit info("OK");
}
//--------------------------------------------------------------------------------
#ifdef USE_COMMUNICATIONS
void Simple_PTZ_widget::create_socket(void)
{
    socket = new QLocalSocket(this);

    connect(socket,         &QLocalSocket::readyRead,
            this,           &Simple_PTZ_widget::readFortune);
    connect(socket,         QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::errorOccurred),
            this,           &Simple_PTZ_widget::displayError);

    connect(ui->btn_server_connect,     &QPushButton::clicked,
            this,                       &Simple_PTZ_widget::server_connect);
    connect(ui->btn_server_disconnect,  &QPushButton::clicked,
            this,                       &Simple_PTZ_widget::server_disconnect);
}
#endif
//--------------------------------------------------------------------------------
#ifdef USE_COMMUNICATIONS
void Simple_PTZ_widget::displayError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        emit error("ServerNotFoundError");
        break;

    case QLocalSocket::ConnectionRefusedError:
        emit error("ConnectionRefusedError");
        break;

    case QLocalSocket::PeerClosedError:
        emit error("PeerClosedError");
        break;

    default:
        emit error(QString("Unknown error: %1")
                   .arg(socket->errorString()));
    }
}
#endif
//--------------------------------------------------------------------------------
#ifdef USE_COMMUNICATIONS
void Simple_PTZ_widget::readFortune(void)
{
    emit info(QString("Simple_PTZ_widget: read %1 bytes")
              .arg(socket->bytesAvailable()));
    emit info(QString("Read data [%1]")
              .arg(socket->readAll().data()));
}
#endif
//--------------------------------------------------------------------------------
#ifdef USE_COMMUNICATIONS
void Simple_PTZ_widget::server_connect(void)
{
    emit trace(Q_FUNC_INFO);

    if(ui->le_server->text().isEmpty())
    {
        emit error("Server name is empty!");
        return;
    }

    emit info("Connect...");

    socket->abort();
    socket->connectToServer(ui->le_server->text());
}
#endif
//--------------------------------------------------------------------------------
#ifdef USE_COMMUNICATIONS
void Simple_PTZ_widget::server_disconnect(void)
{
    emit trace(Q_FUNC_INFO);

    emit info("Disconnect...");

    socket->abort();
    socket->close();
}
#endif
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
    ui->le_login->setText(load_string(P_LOGIN));
    ui->le_password->setText(load_string(P_PASSWORD));
    ui->le_other_cmd->setText(load_string(P_OTHER_CMD));
    ui->le_server->setText(load_string(P_SERVER));

    QUrl url;
    url.setHost(load_string(P_HOST));

    int t_port = 0;
    if(load_int(P_PORT, &t_port))
    {
        url.setPort(t_port);
    }
    ui->ipv4_widget->set_url(url);
}
//--------------------------------------------------------------------------------
void Simple_PTZ_widget::save_setting(void)
{
    save_string(P_LOGIN,        ui->le_login->text());
    save_string(P_PASSWORD,     ui->le_password->text());
    save_string(P_OTHER_CMD,    ui->le_other_cmd->text());
    save_string(P_SERVER,       ui->le_server->text());
    save_string(P_HOST,         ui->ipv4_widget->get_url().host());
    save_int(P_PORT,            ui->ipv4_widget->get_url().port());
}
//--------------------------------------------------------------------------------
