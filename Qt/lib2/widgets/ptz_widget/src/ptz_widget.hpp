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
#include <QWidget>
//--------------------------------------------------------------------------------
#include <QNetworkAccessManager>
#include <QNetworkRequest>
//--------------------------------------------------------------------------------
namespace Ui {
    class PTZ_widget;
}
//--------------------------------------------------------------------------------
class MainBox;
//--------------------------------------------------------------------------------
class PTZ_widget : public QWidget
{
    Q_OBJECT

public:
    explicit PTZ_widget(QWidget *parent = 0);
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

    void f_wiper_on(void);
    void f_wiper_off(void);
    void f_light_on(void);
    void f_light_off(void);
    void f_diaphragm_plus(void);
    void f_diaphragm_minus(void);
    void f_zoom_plus(void);
    void f_zoom_minus(void);
    void f_set_brightness(int value);
    void f_set_contrast(int value);
    void f_set_tone(int value);
    void f_set_saturation(int value);
    void f_set_iris(int value);
    void f_set_shutter(int value);
    void f_set_gamma(int value);
    void f_set_sharpness(int value);
    void f_set_noise(int value);

private:
    Ui::PTZ_widget *ui;
    QMediaPlayer *player = 0;

    QToolButton *btn_lu = 0;
    QToolButton *btn_ru = 0;

    QToolButton *btn_u = 0;
    QToolButton *btn_d = 0;
    QToolButton *btn_l = 0;
    QToolButton *btn_r = 0;

    QToolButton *btn_ld = 0;
    QToolButton *btn_rd = 0;

    QTcpSocket *tcpSocket = 0;
    QNetworkRequest request;
    QNetworkAccessManager networkManager;

    QUrl url;

    void send_cmd(QString cmd,
                  QString func,
                  QVariant param_1,
                  QVariant param_2);
    bool f_connect(void);
    void f_disconnect(void);

    void create_player(void);
    void create_tcp_socket(void);
    void create_position_widgets(void);

    void init(void);
    void updateText(void);

protected:
    void resizeEvent (QResizeEvent *);
};
//--------------------------------------------------------------------------------
#endif // PTZ_WIDGET_HPP
