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
#include "alien.hpp"
//--------------------------------------------------------------------------------
enum {
    RUN_RIGHT = 0,
    RUN_BOTTOM,
    RUN_LEFT,
    RUN_TOP
};
//--------------------------------------------------------------------------------
Alien::Alien(bool is_alive, int pos_x, int pos_y)
{
    alive = is_alive;
    this->pos_x = pos_x;
    this->pos_y = pos_y;

    init();
}
//--------------------------------------------------------------------------------
bool Alien::is_alive(void)
{
    return alive;
}
//--------------------------------------------------------------------------------
void Alien::init(void)
{
    state = RUN_RIGHT;
}
//--------------------------------------------------------------------------------
int Alien::get_x(void)
{
    return pos_x;
}
//--------------------------------------------------------------------------------
int Alien::get_y(void)
{
    return pos_y;
}
//--------------------------------------------------------------------------------
void Alien::run(void)
{
    switch(state)
    {
    case RUN_RIGHT:
        state = RUN_BOTTOM;
        break;

    case RUN_BOTTOM:
        state = RUN_LEFT;
        break;

    case RUN_LEFT:
        state = RUN_TOP;
        break;

    case RUN_TOP:
        state = RUN_RIGHT;
        break;

    default:
        state = RUN_RIGHT;
        break;
    }
}
//--------------------------------------------------------------------------------
