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
#ifndef PALETTE_HPP
#define PALETTE_HPP
//--------------------------------------------------------------------------------
#include <QGridLayout>
#include <QGroupBox>
//--------------------------------------------------------------------------------
#define MAX_BUTTONS_X    256
#define MAX_BUTTONS_Y    256
//--------------------------------------------------------------------------------
class Diod;
//--------------------------------------------------------------------------------
class Palette : public QGroupBox
{
    Q_OBJECT

public:
    explicit Palette(QWidget *parent = nullptr);
    virtual ~Palette();

    void set_data(QByteArray data);
    void set_param(int size_x, int size_y, int led_w, int led_h);
    QByteArray get_data(void);

    void set_left_btn_active(bool value);
    void set_right_btn_active(bool value);
    void set_flag_is_palette(bool value);

    void load_setting(void);
    void save_setting(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    Diod *a_diod[MAX_BUTTONS_X][MAX_BUTTONS_Y];
    QGridLayout *grid;

    int max_x = 4;
    int max_y = 4;
    int led_width = 32;
    int led_height = 32;

    bool flag_active = false;
};
//--------------------------------------------------------------------------------
#endif
