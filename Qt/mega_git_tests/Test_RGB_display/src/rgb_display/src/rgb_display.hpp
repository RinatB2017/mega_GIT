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
#ifndef RGB_DISLPAY_HPP
#define RGB_DISLPAY_HPP
//--------------------------------------------------------------------------------
#include <QFileDialog>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "rgb_display_led.hpp"
//--------------------------------------------------------------------------------
class RGB_display : public MyWidget
{
    Q_OBJECT

signals:
    void send(QString);

public:
    explicit RGB_display(QWidget *parent = nullptr);
    virtual ~RGB_display();

    bool set_param(int cnt_led_x,
                   int cnt_led_y,
                   double width_led,
                   double height_led,
                   double l_border,
                   double u_border);
    bool get_param(int *cnt_led_x,
                   int *cnt_led_y,
                   double *width_led,
                   double *height_led,
                   double *l_border,
                   double *u_border);

    void show_picture(int begin_x, int begin_y);

    int get_max_x(void);
    int get_max_y(void);

    int get_picture_w(void);
    int get_picture_h(void);

    void load_leds(void);
    void save_leds(void);

    bool set_brightness(int value);

public slots:
    bool load_ico(void);
    bool load_pic(void);

    void send_test_data(void);

private slots:
    void set_display(void);
    void get_display(void);
    void set_default(void);

private:
    QGridLayout *grid;
    QList<RGB_dislpay_led *> l_buttons;

    QImage picture;
    int max_x = 0;
    int max_y = 0;
    int begin_x = 0;
    int begin_y = 0;

    double w_led = 0;
    double h_led = 0;

    double left_border = 0;
    double up_border = 0;

    int brightness = 0;

    void init(void);
    void clean_grid(void);
    void create_new_display(void);
    bool load_picture(QString fileName);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // RGB_DISLPAY_HPP
