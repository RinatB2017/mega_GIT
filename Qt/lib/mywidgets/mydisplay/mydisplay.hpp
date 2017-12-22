/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#ifndef MYDISPLAY_HPP
#define MYDISPLAY_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QColor>
//--------------------------------------------------------------------------------
class MyDisplay : public QWidget
{
    Q_OBJECT
public:
    MyDisplay(QWidget *parent = 0);
    bool display(unsigned int value);

    void set_color(QColor color);
    void set_background_color(QColor color);

private:
    int display_width;
    int display_height;
    int border_width;

    QColor color;
    QColor background_color;

    bool a, b, c, d, e, f, g, h;

    void draw(unsigned int value);
    void set_width(unsigned int width);
    void set_height(unsigned int height);

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
