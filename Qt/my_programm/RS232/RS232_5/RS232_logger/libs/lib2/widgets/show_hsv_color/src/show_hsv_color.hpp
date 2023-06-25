/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#ifndef SHOW_HSV_COLOR_HPP
#define SHOW_HSV_COLOR_HPP
//--------------------------------------------------------------------------------
#include <QMouseEvent>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class Show_HSV_color;
}
//--------------------------------------------------------------------------------
class Show_HSV_color : public MyWidget
{
    Q_OBJECT

public:
    explicit Show_HSV_color(QWidget *parent = nullptr);
    virtual ~Show_HSV_color();

    void set_color(QColor color);
    QColor get_color(void);

private:
    Ui::Show_HSV_color *ui;

    void init(void);
    void update_color_RGB(void);
    void update_color_HSV(void);
    void update_image(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);

    void show_H(int value);
    void show_S(int value);
    void show_V(int value);

    void show_R(int value);
    void show_G(int value);
    void show_B(int value);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // SHOW_HSV_COLOR_HPP
