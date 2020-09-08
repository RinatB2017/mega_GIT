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
#include <QCoreApplication>
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "waiting.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Waiting::Waiting(QObject *parent) :
    QObject(parent)
{

}
//--------------------------------------------------------------------------------
bool Waiting::is_loaded(bool *is_loaded, int timeout_msec)
{
    QElapsedTimer time;

    time.start();
    while(*is_loaded == false)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > timeout_msec)
        {
            return E_ERROR_TIMEOUT;
        }
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void Waiting::sec(int timeout_sec)
{
    QElapsedTimer time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > (timeout_sec * 1000))
        {
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void Waiting::msec(int timeout_msec)
{
    QElapsedTimer time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > timeout_msec)
        {
            break;
        }
    }
}
//--------------------------------------------------------------------------------
