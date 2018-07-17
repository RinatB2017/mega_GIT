/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef LED_DISPLAY_HPP
#define LED_DISPLAY_HPP
//--------------------------------------------------------------------------------
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsView>
//--------------------------------------------------------------------------------
#define MAX_WIDTH   2000
#define MAX_HEIGHT  1000

#define MIN_WIDTH   1
#define MIN_HEIGHT  1

#define MAX_LED_WIDTH   1000
#define MAX_LED_HEIGHT  1000

#define MIN_LED_WIDTH   2
#define MIN_LED_HEIGHT  2

#define DEFAULT_LED_WIDTH   32
#define DEFAULT_LED_HEIGHT  32
//--------------------------------------------------------------------------------
class LED_display : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LED_display(unsigned int max_x,
                         unsigned int max_y,
                         unsigned int led_width,
                         unsigned int led_height,
                         QWidget *parent = nullptr);

    unsigned int get_max_x(void);
    unsigned int get_max_y(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

public slots:
    bool draw_led(unsigned int x, unsigned int y, QColor color);
    void show_param(void);

private:
    QAbstractGraphicsShapeItem *items[MAX_WIDTH][MAX_HEIGHT] = { {0, 0} };
    QGraphicsScene *scene = 0;

    unsigned int max_x = 0;
    unsigned int max_y = 0;
    unsigned int led_width = 0;
    unsigned int led_height = 0;
};
//--------------------------------------------------------------------------------
#endif // LED_DISPLAY_HPP
