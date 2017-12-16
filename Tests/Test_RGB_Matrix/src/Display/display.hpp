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
#include <QFrame>
//--------------------------------------------------------------------------------
#define MAX_DISPLAY_X    128
#define MAX_DISPLAY_Y    32
//--------------------------------------------------------------------------------
#define DEFAULT_X       16
#define DEFAULT_Y       8
//--------------------------------------------------------------------------------
class Diod;
//--------------------------------------------------------------------------------
class Display : public QFrame
{
    Q_OBJECT

public:
    Display(int max_x,
            int max_y,
            QWidget *parent);
    ~Display();

    bool set_color(int x,
                   int y,
                   QColor color);
    bool set_color(int x,
                   int y,
                   uint8_t R_value,
                   uint8_t G_value,
                   uint8_t B_value);

    bool get_R(int x, int y, uint8_t *value);
    bool get_G(int x, int y, uint8_t *value);
    bool get_B(int x, int y, uint8_t *value);

    QByteArray get_data(void);

    void set_active(bool value);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

private:
    Diod *diod[MAX_DISPLAY_X][MAX_DISPLAY_Y];
    int max_x = 0;
    int max_y = 0;

    bool flag_active = false;

    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
