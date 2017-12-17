/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QWidget>
//--------------------------------------------------------------------------------
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MainBox;
}
//--------------------------------------------------------------------------------
class MySplashScreen;
class SerialBox5;
class QToolButton;
class QSpinBox;
class QToolBar;
class Button;
class Diod;
//--------------------------------------------------------------------------------
class MainBox : public MyWidget
{
    Q_OBJECT

public:
    explicit MainBox(QWidget *parent,
                     MySplashScreen *splash);
    ~MainBox();

signals:
    void send(QByteArray);

private slots:
    void on(void);
    void off(void);

    void test(void);

    void read_data(QByteArray ba);

    void click(void);

    void btn_click(bool state);
    void btn_click_adv(bool state);

    void send_grid_question(int value);

    void send_hot_pic_question(int value);
    void send_cold_pic_question(int value);

private:
    MySplashScreen *splash = 0;
    Ui::MainBox *ui = 0;
    SerialBox5 *serialBox = 0;
    QByteArray data_rs232;

    uint8_t buf_leds[MAX_SCREEN_X][MAX_SCREEN_Y] = { \
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0
    };

    qreal center_x;
    qreal center_y;
    qreal center_r;
    qreal led_r;
    qreal min_r;
    qreal max_r;
    qreal min_angle;
    qreal max_angle;
    int   inc_r;
    qreal temp_x = 0;
    qreal temp_y = 0;

    QList<QToolButton *> grid_buttons;
    QList<Button *>      pic_buttons;

    void init(void);
    void init_widgets(void);
    void connect_log(void);

    QToolButton *btn[MAX_SCREEN_X][MAX_SCREEN_Y];

    QSlider  *sl_value = 0;
    QSpinBox *sb_value = 0;

    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y);

    void set_property(int btn_index, quint16 value);

    void createTestBar(void);
    void createSerialBox(void);
    void createGridBox(void);

    QString convert_data_to_ascii(uint8_t data);
    uint8_t convert_ascii_to_value(char hi, char lo);

protected:
    void changeEvent(QEvent *event);

};
//--------------------------------------------------------------------------------
#endif // MAINBOX_HPP
