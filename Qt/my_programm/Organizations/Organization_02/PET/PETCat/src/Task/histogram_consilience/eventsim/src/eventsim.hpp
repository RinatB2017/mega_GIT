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
#ifndef EVENTSIM_HPP
#define EVENTSIM_HPP
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#include "gauss.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
class EventSim : public QObject
{
    Q_OBJECT
public:
    explicit EventSim(QObject *parent = nullptr);

    explicit EventSim(int m, double dev);
    pet_event Next(void);

private:
    Gauss *g;
    char src = 2;

    uint tsA = 10000;
    uint tsB = 10000;
};
//--------------------------------------------------------------------------------
#endif // EVENTSIM_HPP
