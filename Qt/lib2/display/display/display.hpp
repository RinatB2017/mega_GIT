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
#ifndef DISPLAY_HPP
#define DISPLAY_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define MAX_DISPLAY_X    256
#define MAX_DISPLAY_Y    256
//--------------------------------------------------------------------------------
#define DEFAULT_X       16
#define DEFAULT_Y       8
//--------------------------------------------------------------------------------
class Diod;
//--------------------------------------------------------------------------------
class Display : public MyWidget
{
    Q_OBJECT

public:
    Display(unsigned int max_x,
            unsigned int max_y,
            unsigned int led_width = 32,
            unsigned int led_height = 32,
            QWidget *parent = 0);
    ~Display();

    bool set_color(unsigned int x,
                   unsigned int y,
                   QColor color);
    bool set_color(unsigned int x,
                   unsigned int y,
                   uint8_t R_value,
                   uint8_t G_value,
                   uint8_t B_value);

    int get_max_x(void);
    int get_max_y(void);

    bool get_R(unsigned int x,
               unsigned int y,
               uint8_t *value);
    bool get_G(unsigned int x,
               unsigned int y,
               uint8_t *value);
    bool get_B(unsigned int x,
               unsigned int y,
               uint8_t *value);

    void set_data(QByteArray data);
    QByteArray get_data(void);

    void set_left_btn_active(bool value);
    void set_right_btn_active(bool value);

    void load_setting(void);
    void save_setting(void);

    void clear(void);

    bool resize(unsigned int w,
                unsigned int h);
    bool resize_led(unsigned int w,
                    unsigned int h);

private:
    Diod *diod[MAX_DISPLAY_X][MAX_DISPLAY_Y];
    unsigned int max_x = 0;
    unsigned int max_y = 0;

    bool flag_active = false;

    bool create_display(unsigned int w,
                        unsigned int h,
                        unsigned int led_width,
                        unsigned int led_height);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
