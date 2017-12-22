/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef FLOODMAP_GLASS_HPP
#define FLOODMAP_GLASS_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
class FloodMap_glass : public MyWidget
{
public:
    FloodMap_glass(QWidget *parent = 0);
    void install(QWidget* widget);

    void get_circle(QPoint *c, int *r);
    void get_rect(int *x1, int *x2, int *y1, int *y2);

protected:
    bool eventFilter(QObject* object, QEvent* event);
    void paintEvent(QPaintEvent *);

private:
    QPoint center;
    int radius;
};
//--------------------------------------------------------------------------------
#endif
