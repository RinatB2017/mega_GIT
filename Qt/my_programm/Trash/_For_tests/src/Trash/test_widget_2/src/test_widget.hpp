/*********************************************************************************
**                                                                              **
**     Copyright (C) 2019                                                       **
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
#ifndef TEST_WIDGET_HPP
#define TEST_WIDGET_HPP
//--------------------------------------------------------------------------------
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(int l_width,
                        int l_height,
                        QWidget *parent = nullptr);

    void set_color(int x, int y, QColor color);

private:
    int d_width = 0;
    int d_height = 0;

    int cnt_x = 0;
    int cnt_y = 0;

    int led_width = 16;
    int led_height = 16;
    int led_border = 1;

    QList<QColor> leds;

    QColor led_color = Qt::black;
    QColor border_color = Qt::gray;

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif // TEST_WIDGET_HPP
