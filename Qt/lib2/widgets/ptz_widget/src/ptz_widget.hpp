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
#ifndef PTZ_WIDGET_HPP
#define PTZ_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QMediaPlayer>
#include <QToolButton>
#include <QSettings>
#include <QWidget>
//--------------------------------------------------------------------------------
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
struct PTZ_PARAM
{
    QString cmd;
    QString func;
    QVariant param1;
    QVariant param2;

    QString btn_caption;
    int min_value;
    int max_value;
};
//--------------------------------------------------------------------------------
namespace Ui {
    class PTZ_widget;
}
//--------------------------------------------------------------------------------
class MainBox;
//--------------------------------------------------------------------------------
class PTZ_widget : public MyWidget
{
    Q_OBJECT

public:
    explicit PTZ_widget(QWidget *parent = nullptr);
    ~PTZ_widget();

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    void play(void);
    void pause(void);
    void stop(void);

private slots:
    void choice(void);
    void f_error(QMediaPlayer::Error err);
    void s_error(QAbstractSocket::SocketError err);
    void s_state(QAbstractSocket::SocketState state);

    void read_data(void);

    void f_connected(void);
    void f_disconnected(void);

    void f_stop(void);

    void f_left_up(void);
    void f_right_up(void);

    void f_left(void);
    void f_right(void);
    void f_up(void);
    void f_down(void);

    void f_left_down(void);
    void f_right_down(void);

    void f_test(void);

    void f_push_button(void);
    void f_move_slider(void);

    //---
    void add_buttons(int index,
                     QString name,
                     PTZ_PARAM param1,
                     PTZ_PARAM param2);
    void add_slider(int index,
                    QString name,
                    PTZ_PARAM params);
    //---

private:
    Ui::PTZ_widget *ui;
    QMediaPlayer *player;
    int port = 81;

    QSettings *settings;

    QTcpSocket *tcpSocket;
    QNetworkRequest request;
    QNetworkAccessManager networkManager;

    QUrl url;

    QList<PTZ_PARAM> l_params;

    void send_cmd(QString cmd,
                  QString func,
                  QVariant param_1,
                  QVariant param_2);
    bool f_connect(void);
    void f_disconnect(void);

    void create_player(void);
    void create_tcp_socket(void);
    void connect_position_widgets(void);

    void load_widgets(QString group_name);
    void save_widgets(QString group_name);

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // PTZ_WIDGET_HPP
