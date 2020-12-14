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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMediaPlayer>
#include <QVideoProbe>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define P_LOGIN     "login"
#define P_PASSWORD  "password"
#define P_HOST      "host"
#define P_PORT      "port"
//--------------------------------------------------------------------------------
namespace Ui {
    class Simple_PTZ_widget;
}
//--------------------------------------------------------------------------------
class Simple_PTZ_widget : public MyWidget
{
    Q_OBJECT

signals:
    void get_frame(QVideoFrame);

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

    void f_left_right(void);
    void f_up_down(void);

private:
    Ui::Simple_PTZ_widget   *ui;
    QPointer<QMediaPlayer>  player;
    QPointer<QVideoProbe>   probe;

    QNetworkRequest request;
    QNetworkAccessManager networkManager;

    void processFrame(QVideoFrame const &frame);

    void send_cmd(QString cmd);
    void create_player(void);
    void connect_position_widgets(void);

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // PTZ_WIDGET_HPP
