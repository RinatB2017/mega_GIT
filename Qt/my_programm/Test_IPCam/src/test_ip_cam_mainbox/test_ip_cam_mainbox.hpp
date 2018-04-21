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
#ifndef MAINBOX_HPP
#define MAINBOX_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include <QMediaPlayer>
//--------------------------------------------------------------------------------
#include <QNetworkAccessManager>
#include <QNetworkRequest>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class QToolButton;
class QToolBar;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    MainBox(QWidget *parent,
            MySplashScreen *splash);
    ~MainBox();

signals:
    void send(QByteArray);

public slots:
    void f_video(void);

private slots:
    void choice_test(void);

    bool test(void);

    bool test_OPTIONS(void);
    bool test_DESCRIBE(void);
    bool test_SET_PARAMETER(void);
    bool test_GET_PARAMETER(void);
    bool test_SETUP(void);
    bool test_TEARDOWN(void);
    bool test_PLAY(void);
    bool test_PAUSE(void);

    void readFortune(void);
    void s_error(QAbstractSocket::SocketError err);
    void f_error(QMediaPlayer::Error err);

    void f_connect(void);
    void f_disconnect(void);

    void f_connected(void);
    void f_disconnected(void);

    void testbar_lock(void);
    void testbar_unlock(void);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6,
        ID_TEST_7
    };

    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (MainBox::*func)(void);
    } CMD_t;
    QComboBox *cb_test = 0;
    QToolButton *btn_choice_test = 0;
    QToolButton *btn_test = 0;
    QList<CMD> commands;

    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;

    QMediaPlayer *player = 0;

    QTcpSocket *tcpSocket = 0;
    QNetworkRequest request;
    QNetworkAccessManager networkManager;

    QPushButton *btn_connect = 0;
    QPushButton *btn_disconnect = 0;

    bool send_data(QByteArray data);

    void init(void);
    void createTestBar(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
