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
#ifndef SIMPLE_PTZ_WIDGET_HPP
#define SIMPLE_PTZ_WIDGET_HPP
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
    class Simple_PTZ_widget;
}
//--------------------------------------------------------------------------------
class MainBox;
//--------------------------------------------------------------------------------
class Simple_PTZ_widget : public MyWidget
{
    Q_OBJECT

public:
    explicit Simple_PTZ_widget(QWidget *parent = nullptr);
    ~Simple_PTZ_widget();

public slots:
    void play(void);
    void pause(void);
    void stop(void);

private slots:
    void onFinished( QNetworkReply* reply );

    void f_error(QMediaPlayer::Error err);

    void f_stop(void);

    void f_left(void);
    void f_right(void);
    void f_up(void);
    void f_down(void);

private:
    Ui::Simple_PTZ_widget *ui;
    QMediaPlayer *player = nullptr;

    //int port = 554;
    int port = 81;

    QNetworkRequest request;
    QNetworkAccessManager networkManager;

    QUrl url;

    QList<PTZ_PARAM> l_params;

    void send_cmd(QString cmd);

    bool f_connect(void);
    void f_disconnect(void);

    void create_player(void);
    //void create_tcp_socket(void);
    void connect_position_widgets(void);

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // PTZ_WIDGET_HPP
