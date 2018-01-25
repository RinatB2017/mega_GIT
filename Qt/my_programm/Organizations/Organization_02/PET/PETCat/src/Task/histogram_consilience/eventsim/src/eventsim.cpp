/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include "eventsim.hpp"
//--------------------------------------------------------------------------------
EventSim::EventSim(QObject *parent) : QObject(parent)
{

}
//--------------------------------------------------------------------------------
EventSim::EventSim(int m, double dev)
{
    g = new Gauss(m, dev);
}
//--------------------------------------------------------------------------------
pet_event EventSim::Next(void)
{
    pet_event e;
    e.src = src;
    if (src == 2)
    {
        //tsA = (uint)(tsB + (int)g.Next());
        tsA += 10000;
        e.src = src;
        e.ts = tsA;
        src = 3;
    }
    else
    {
        tsB = (uint)((int)tsA + g->Next());
        // tsA += dd;
        e.src = src;
        e.ts = tsB;
        src = 2;
    }
    return e;
}
//--------------------------------------------------------------------------------
