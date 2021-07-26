/*********************************************************************************
**                                                                              **
**     Copyright (C) 2021                                                       **
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
#ifndef ANALOG_CLOCK_HPP
#define ANALOG_CLOCK_HPP
//--------------------------------------------------------------------------------
#include <QFontMetrics>
#include <QPainter>
#include <QWidget>
#include <QtMath>
#include <QFont>
//--------------------------------------------------------------------------------
class Analog_clock : public QWidget
{
    Q_OBJECT

public:
    explicit Analog_clock(QWidget *parent = nullptr);
    ~Analog_clock();

private:
    QFont font;
    int font_size = 24;

    void init(void);
    void calc_line(qreal center_x,
                   qreal center_y,
                   qreal angle,
                   qreal radius,
                   qreal *end_x,
                   qreal *end_y);

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif // ANALOG_CLOCK_HPP
