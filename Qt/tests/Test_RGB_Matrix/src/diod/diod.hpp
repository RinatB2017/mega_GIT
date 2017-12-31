/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef DIOD_HPP
#define DIOD_HPP
//--------------------------------------------------------------------------------
#include <QToolButton>
//--------------------------------------------------------------------------------
#define WIDTH  32
#define HEIGHT 32
//--------------------------------------------------------------------------------
class Diod : public QToolButton
{
    Q_OBJECT

public:
    Diod(QWidget *parent);

    void set_color(QColor color);
    void set_color(uint8_t R_value,
                   uint8_t G_value,
                   uint8_t B_value);

    QRgb get_color(void);

    uint8_t get_R(void);
    uint8_t get_G(void);
    uint8_t get_B(void);

    void set_left_btn_active(bool value);
    void set_right_btn_active(bool value);
    void set_flag_is_palette(bool value);

    bool resize(int w, int h);

private:
    uint8_t R_color = 0;
    uint8_t G_color = 0;
    uint8_t B_color = 0;

    bool active_left_btn = false;
    bool active_right_btn = false;
    bool is_palette = false;

    void set_cursor(void);

protected:
    void mousePressEvent(QMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif
