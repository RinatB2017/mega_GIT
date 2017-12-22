/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#include "ship.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
Ship::Ship(bool is_rand_index)
{
    for(int n=0; n<31; n++)
    {
        p_animation.append(QPixmap(QString(ID_SHIP).arg(n + 4000)));
    }
    if(is_rand_index)
    {
        index = rand() % 30;
    }
    setPixmap(p_animation.at(index));
}
//--------------------------------------------------------------------------------
void Ship::animation(void)
{
    corr++;
    if(corr < 10)
    {
        return;
    }
    corr = 0;

    setPixmap(p_animation.at(index));
    if(index < 30)
        index++;
    else
        index = 0;
}
//--------------------------------------------------------------------------------
