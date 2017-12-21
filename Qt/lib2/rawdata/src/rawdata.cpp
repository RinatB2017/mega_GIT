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
//--------------------------------------------------------------------------------
#include "rawdata.hpp"
//--------------------------------------------------------------------------------
RawData::RawData(QObject *parent,
                 unsigned char begin_simvol,
                 unsigned char end_simvol,
                 bool append_end_simvol) :
    QObject(parent),
    begin_simvol(begin_simvol),
    end_simvol(end_simvol),
    append_end_simvol(append_end_simvol)
{
    queue.clear();
}
//--------------------------------------------------------------------------------
void RawData::raw(const QByteArray &data)
{
    int n;
    bool found;

    for(n=0; n<data.length(); n++)
        queue.enqueue(data.at(n));
    found = false;
    while(queue.isEmpty() == false)
    {
        if(queue.value(0) == begin_simvol)
        {
            found = true;
            break;
        }
        else
        {
            queue.removeAt(0);
        }
    }
    if(found == false)
        return;

    found = false;
    for(n=0; n<queue.size(); n++)
    {
        if(queue.value(n) == end_simvol)
        {
            found = true;
            break;
        }
    }
    if(found == false)
        return;

    clear_data.clear();
    while(queue.isEmpty() == false)
    {
        if(queue.value(0) == end_simvol)
        {
            if(append_end_simvol)
                clear_data.append(queue.at(0));
            queue.clear();
            break;
        }
        else
        {
            clear_data.append(queue.at(0));
        }
        queue.removeAt(0);
    }
    emit clear(clear_data);
}
//--------------------------------------------------------------------------------
