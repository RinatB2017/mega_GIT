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
#include "eventcoincidencegen.hpp"
#include "gauss.hpp"
//--------------------------------------------------------------------------------
EventCoincidenceGen::EventCoincidenceGen(QObject *parent) : QObject(parent)
{

}
//--------------------------------------------------------------------------------
pet_event* EventCoincidenceGen::Generate(int num,
                                         char srcA,
                                         char srcB,
                                         double mean,
                                         double variance)
{
    pet_event* events = new pet_event[num];

    double ts = 0;

    Gauss *g = new Gauss(mean, variance);
    //Random r = new Random();

    for (int i = 0; i < num; )
    {
        /**/
        ts = (i + 1) * 1000.0;
        events[i].src = srcA;
        events[i].flags = 0;
        events[i].ts = (uint32_t)ts;
        events[i].ts_fract = (uint16_t)((double)UINT16_MAX * (double)(ts - (uint32_t)ts));
        events[i].xp = 0;
        events[i].xn = 0;
        events[i].yp = 0;
        events[i].yn = 0;
        i++;

        /**/
        ts += g->Next() /* + 0.75*/;
        events[i].src = srcB;
        events[i].flags = 0;
        events[i].ts = (uint32_t)ts;
        events[i].ts_fract = (uint16_t)((double)UINT16_MAX * (double)(ts - (uint32_t)ts));
        events[i].xp = 0;
        events[i].xn = 0;
        events[i].yp = 0;
        events[i].yn = 0;
        i++;
    }
    return events;
}
//--------------------------------------------------------------------------------
