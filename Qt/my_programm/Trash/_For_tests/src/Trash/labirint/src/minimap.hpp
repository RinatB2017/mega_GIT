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
#ifndef MINIMAP_HPP
#define MINIMAP_HPP
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "world.hpp"
#include "cell.hpp"

#include "defines.hpp"
//--------------------------------------------------------------------------------
class Minimap : public MyWidget
{
    Q_OBJECT

public:
    explicit Minimap(QWidget *parent = nullptr);

signals:
    void cell_click(uint8_t value);

private:
    Cell *cw[MAX_X][MAX_Y];

    void init(void);
    void updateText(void);

protected:
    void mousePressEvent(QMouseEvent *event);
};
//--------------------------------------------------------------------------------
#endif
