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
#ifndef TEST_GLASS_HPP
#define TEST_GLASS_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
typedef struct point
{
    QPointF center;
    qreal   radius;
    QColor  color;
} point_t;
//--------------------------------------------------------------------------------
#define MAX_POINT   (3*6+1)
//--------------------------------------------------------------------------------
class Test_Glass : public MyWidget
{
    Q_OBJECT

public:
    explicit Test_Glass(QWidget *parent);
    virtual ~Test_Glass();

    virtual void install(QWidget* widget);

    void test(void);

protected:
    bool eventFilter(QObject* object, QEvent* event);
    void paintEvent(QPaintEvent *);

private:
    point points[MAX_POINT];

    qreal center_x;
    qreal center_y;
    qreal center_r;
    qreal led_r;
    qreal min_r;
    qreal max_r;
    qreal min_angle;
    qreal max_angle;
    int   inc_r;
    qreal temp_x = 0;
    qreal temp_y = 0;

    void connect_log(void);

    qreal rad_to_grad(qreal rad);
    qreal grad_to_rad(qreal grad);

    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y);
};
//--------------------------------------------------------------------------------
#endif
