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
#include <QObject>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "processor.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
Processor::Processor(QWidget *parent) :
    MyWidget(parent)
{

}
//--------------------------------------------------------------------------------
void Processor::input(const QByteArray &data)
{
#if 0
    emit output(data);
#else
    Q_UNUSED(data)
    emit output(QByteArray("ok"));
#endif
}
//--------------------------------------------------------------------------------
void Processor::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Processor::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Processor::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Processor::save_setting(void)
{

}
//--------------------------------------------------------------------------------
