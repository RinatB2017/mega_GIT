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
#include <QCoreApplication>
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
#include "ad8400.hpp"
#include "sleeper.h"
#include "at93c56.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
//declare for MPSSE command
const BYTE MSB_RISING_EDGE_CLOCK_BYTE_OUT   = 0x10;
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT  = 0x11;
const BYTE MSB_RISING_EDGE_CLOCK_BIT_OUT    = 0x12;
const BYTE MSB_FALLING_EDGE_CLOCK_BIT_OUT   = 0x13;
const BYTE MSB_RISING_EDGE_CLOCK_BYTE_IN    = 0x20;
const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN     = 0x22;
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_IN   = 0x24;
const BYTE MSB_FALLING_EDGE_CLOCK_BIT_IN    = 0x26;
//--------------------------------------------------------------------------------
AD8400::AD8400(I2C_Freq freq,
               QWidget *parent) :
    MyWidget(parent)
{
    ft2232h = new FT2232H(freq, parent);
}
//--------------------------------------------------------------------------------
AD8400::~AD8400()
{
    ft2232h->close();
    ft2232h->deleteLater();
}
//--------------------------------------------------------------------------------
FT_STATUS AD8400::open(int deviceNumber)
{
    return ft2232h->open(deviceNumber);
}
//--------------------------------------------------------------------------------
void AD8400::close(void)
{
    ft2232h->close();
}
//--------------------------------------------------------------------------------
BOOL AD8400::write(BYTE data)
{
    emit debug(QString("send %1").arg(data));
    ft2232h->clear_data();
    //ft2232h->append_data(data);

    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7);                    // 7+1 = 8
    ft2232h->append_data(BYTE(data >> 8));      // Set 0xValueH

    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7);                    // 7+1 = 8
    ft2232h->append_data(BYTE(data & 0xFF));    // Set 0xValueL

    return ft2232h->write_data();
}
//--------------------------------------------------------------------------------
void AD8400::test(void)
{
    FT_STATUS ftStatus = FT_OK;

    emit info("test AD8400");
    ftStatus = open(1);
    if(ftStatus == FT_OK)
    {
        for(int n=0; n<255; n++)
        {
            QCoreApplication::processEvents();
            write(n);
            Sleeper::msleep(10);
        }
        close();
    }
    else
    {
        emit error("test failed!");
    }
    emit info("test end");
}
//--------------------------------------------------------------------------------
void AD8400::updateText(void)
{

}
//--------------------------------------------------------------------------------
