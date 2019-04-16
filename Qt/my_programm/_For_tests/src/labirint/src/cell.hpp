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
#ifndef CELL_HPP
#define CELL_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
union CELL {
    uint8_t value;
    struct {
        unsigned b_left  : 1;
        unsigned b_right : 1;
        unsigned b_up    : 1;
        unsigned b_down  : 1;
        unsigned reserved : 4;
    } bites;
};
//--------------------------------------------------------------------------------
class Cell : public QWidget
{
    Q_OBJECT

public:
    explicit Cell(CELL cell,
                  int s_cell,
                  QWidget *parent);

    uint8_t get_value(void);
    void set_value(uint8_t value);

private:
    CELL cell;
    int c_width = 0;
    int c_height = 0;
    int c_border = 0;

    QColor color_cell   = Qt::white;
    QColor color_border = Qt::black;

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
