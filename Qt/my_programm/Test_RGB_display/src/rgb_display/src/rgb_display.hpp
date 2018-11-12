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
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "rgb_display_led.hpp"
//--------------------------------------------------------------------------------
class RGB_display : public MyWidget
{
    Q_OBJECT

public:
    explicit RGB_display(QWidget *parent = nullptr);
    ~RGB_display();

    void show_picture(int begin_x, int begin_y);

    int get_max_x(void);
    int get_max_y(void);

    void load_leds(void);
    void save_leds(void);

public slots:
    bool load_ico(void);
    bool load_pic(void);

private slots:
    void redraw_display(void);

private:
    QGridLayout *grid = nullptr;
    QList<RGB_dislpay_led *> l_buttons;

    QImage picture;
    int max_x = 0;
    int max_y = 0;
    int begin_x = 0;
    int begin_y = 0;

    QSpinBox *sb_max_x;
    QSpinBox *sb_max_y;
    QDoubleSpinBox *dsb_led_width;
    QDoubleSpinBox *dsb_led_height;
    QDoubleSpinBox *dsb_up_border;
    QDoubleSpinBox *dsb_left_border;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // RGB_DISLPAY_HPP
