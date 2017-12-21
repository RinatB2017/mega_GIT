/*********************************************************************************
**                                                                              **
**     Copyright (C) 2013                                                       **
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
#ifndef WAITING_HPP
#define WAITING_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include "defines.hpp"
//--------------------------------------------------------------------------------
class Waiting : public QObject
{
    Q_OBJECT
public:
    explicit Waiting(QObject *parent = 0);

    static bool is_loaded(bool *is_loaded, int timeout_msec = MAX_TIMEOUT);
    static void sec(int timeout_sec);
    static void msec(int timeout_msec);
};
//--------------------------------------------------------------------------------
#endif // WAINTING_HPP
