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
#ifndef MINIMAP_HPP
#define MINIMAP_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define MAX_X 3
#define MAX_Y 3
//--------------------------------------------------------------------------------
class QGridLayout;
//--------------------------------------------------------------------------------
class MiniMap : public MyWidget
{
    Q_OBJECT

public:
    MiniMap(QWidget *parent = 0);
    ~MiniMap();

    bool set(int x, int y, int id);
    bool get(int x, int y, int *id);

private:
    int id_map[MAX_X][MAX_Y];
    QGridLayout *grid_map = 0;

    void init(void);
    bool add_item(int x, int y, int id);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // MINIMAP_HPP
