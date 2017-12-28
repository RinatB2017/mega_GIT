/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "indicator_thread.hpp"
#include "indicator.hpp"
//--------------------------------------------------------------------------------
Indicator::Indicator(I2C_Freq freq,
                     QWidget *parent) :
    MyWidget(parent),
    freq(freq)
{
    init();
}
//--------------------------------------------------------------------------------
void Indicator::init(void)
{

}
//--------------------------------------------------------------------------------
void Indicator::run(void)
{
    emit info("run");

    thread = new Indicator_thread(freq, this);
    connect(thread, SIGNAL(finished()), this, SLOT(measuring_finished()));

    thread->start();
}
//--------------------------------------------------------------------------------
void Indicator::stop(void)
{

}
//--------------------------------------------------------------------------------
void Indicator::measuring_finished(void)
{
    emit info("finished");
    emit finished();
}
//--------------------------------------------------------------------------------
void Indicator::updateText(void)
{

}
//--------------------------------------------------------------------------------
