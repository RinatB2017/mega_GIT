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
    explicit Display(QWidget *parent = nullptr);
    virtual ~Display();

    bool set_color(int x,
                   int y,
                   QColor color);
    bool set_color(int x,
                   int y,
                   uint8_t R_value,
                   uint8_t G_value,
                   uint8_t B_value);
    void set_param(int size_x,
                   int size_y,
                   int led_w,
                   int led_h);
    void get_param(int *size_x,
                   int *size_y,
                   int *led_w,
                   int *led_h);

    uint get_max_x(void);
    uint get_max_y(void);

    bool get_R(int x,
               int y,
               uint8_t *value);
    bool get_G(int x,
               int y,
               uint8_t *value);
    bool get_B(int x,
               int y,
               uint8_t *value);

    void set_data(QByteArray data);
    QByteArray get_data(void);

    void set_left_btn_active(bool value);
    void set_right_btn_active(bool value);

    void clear(void);

    bool resize(int w,
                int h);
    bool resize_led(int w,
                    int h);

    void load_setting(void);
    void save_setting(void);

private:
    Diod *a_diod[MAX_DISPLAY_X][MAX_DISPLAY_Y];
    int max_x = 10;
    int max_y = 10;
    int led_width = 32;
    int led_height = 32;

    bool flag_active = false;

    bool create_display(int w,
                        int h,
                        int led_width,
                        int led_height);
    void updateText(void);
    bool programm_is_exit(void);
};
//--------------------------------------------------------------------------------
#endif
