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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QMediaPlayer>
#include <QNetworkRequest>
#include <QVideoWidget>
//--------------------------------------------------------------------------------
#include "ui_test_ip_cam_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_ip_cam_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
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
    save_widgets("Test_IPCam");
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

    createTestBar();

    //---
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, (SLOT(s_error(QAbstractSocket::SocketError))));
    //---
    player = new QMediaPlayer;
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this,   SLOT(f_error(QMediaPlayer::Error)));
    player->setVideoOutput(ui->video_widget);

    ui->cb_address->addItem("rtsp://192.168.0.66/av0_0");
    ui->cb_address->addItem("rtsp://192.168.1.88:554/HD");

    connect(ui->btn_run,    SIGNAL(clicked(bool)),  this,   SLOT(f_video()));
    //---

    load_widgets("Test_IPCam");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", 0 });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked(bool)), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*my_mega_function)(void);
            my_mega_function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::test(void)
{
    bool ok = false;

    QByteArray reqStr;
    reqStr.append("OPTIONS rtsp://192.168.0.66 RTSP/1.0\r\n");
    //reqStr.append("GET_PARAMETER rtsp://192.168.0.66 RTSP/1.0\r\n");
    //reqStr.append("SETUP rtsp://192.168.0.66 RTSP/1.0\r\n");
    //reqStr.append("PLAY rtsp://192.168.0.66 RTSP/1.0\r\n");
    //reqStr.append("http://192.168.0.66/cgi-bin/network_cgi?action=get&user=admin&pwd=admin");

    reqStr.append("CSeq: 1\r\n");
    reqStr.append("Session: 1\r\n");
    reqStr.append("\r\n");

    tcpSocket->connectToHost("192.168.0.66", 554);

    qint64 bytes = tcpSocket->write(reqStr);
    if(bytes < 0)
    {
        emit error(QString("write bytes %1").arg(bytes));
        return false;
    }
    ok = tcpSocket->waitForBytesWritten(1000);
    if(!ok)
    {
        emit error("waitForBytesWritten");
        return false;
    }
    ok = tcpSocket->waitForReadyRead(1000);
    if(!ok)
    {
        emit error("waitForReadyRead");
        return false;
    }
    emit info("OK");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);

    QUrl url;
    url.setUrl("rtsp://192.168.0.66:554");
    emit info(url.scheme());
    emit info(url.host());
    emit info(QString("%1").arg(url.port()));

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);

    test();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit trace(Q_FUNC_INFO);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit trace(Q_FUNC_INFO);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit trace(Q_FUNC_INFO);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit trace(Q_FUNC_INFO);
    return true;
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

    default:
        emit error(QString("unknown error %1").arg(err));
        break;
    }
    emit error(player->errorString());
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
void MainBox::readFortune(void)
{
    QByteArray ba = tcpSocket->readAll();
    emit info(ba);
}
//--------------------------------------------------------------------------------
void MainBox::s_error(QAbstractSocket::SocketError err)
{
    emit error(QString("err: %1").arg(err));
    emit error(QString("errorString: %1").arg(tcpSocket->errorString()));
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
