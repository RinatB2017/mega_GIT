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
#include <qmath.h>
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
#include "ds18b20.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
union TEMPERATURE
{
    unsigned short value;
    struct
    {
        unsigned bit_0:1;
        unsigned bit_1:1;
        unsigned bit_2:1;
        unsigned bit_3:1;
        unsigned bit_4:1;
        unsigned bit_5:1;
        unsigned bit_6:1;
        unsigned bit_7:1;
        unsigned bit_8:1;
        unsigned bit_9:1;
        unsigned bit_10:1;
        unsigned bit_11:1;
        unsigned bit_12:1;
        unsigned bit_13:1;
        unsigned bit_14:1;
        unsigned bit_15:1;
    } bites;
};

//declare for MPSSE command
const BYTE MSB_RISING_EDGE_CLOCK_BYTE_OUT   = '\x10';
const BYTE MSB_RISING_EDGE_CLOCK_BYTE_IN    = '\x20';

const BYTE MSB_RISING_EDGE_CLOCK_BIT_OUT    = '\x12';
const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN     = '\x22';

const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT  = '\x11';
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_IN   = '\x24';

const BYTE MSB_FALLING_EDGE_CLOCK_BIT_OUT   = '\x13';
const BYTE MSB_FALLING_EDGE_CLOCK_BIT_IN    = '\x26';

const BYTE SET_DATA_BITS_LO_BYTE            = '\x80';
const BYTE SET_DATA_BITS_HI_BYTE            = '\x82';

const BYTE GET_DATA_BITS_LO_BYTE            = '\x81';
const BYTE GET_DATA_BITS_HI_BYTE            = '\x83';


const BYTE READ_ROM                         = 0x33;
const BYTE CONVERT_T                        = 0x44;
const BYTE COPY_SCRATCHPAD                  = 0x48;
const BYTE WRITE_SCRATCHPAD                 = 0x4E;
const BYTE MATCH_ROM                        = 0x55;
const BYTE SKIP_ROM                         = 0xCC;
const BYTE READ_POWER_SUPPLY                = 0xB4;
const BYTE RECALL_E                         = 0xB8;
const BYTE READ                             = 0xBE;
const BYTE ALARM_SEARCH                     = 0xEC;
const BYTE SEARCH_ROM                       = 0xF0;

// данные передаются младшим битом вперед
//--------------------------------------------------------------------------------
DS18B20::DS18B20(I2C_Freq freq, QWidget *parent) :
    QWidget(parent)
{
    connect_log();

    ft2232h = new FT2232H(freq, parent);
}
//--------------------------------------------------------------------------------
DS18B20::~DS18B20()
{
    ft2232h->deleteLater();
}
//--------------------------------------------------------------------------------
void DS18B20::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void DS18B20::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
float DS18B20::convert_9bit(unsigned short  temp_ds18b20)
{
    short temp;
    union TEMPERATURE temper;

    temp = temp_ds18b20 >> 3;

    temper.value = temp;

    int sign = 1;
    if(temper.bites.bit_8)
        sign = -1;

    if(temper.bites.bit_8)
        temp = (~temp + 1);
    temper.value = temp << 3;

    float value = 0;
    if(temper.bites.bit_7) value += qPow(2, 3);
    if(temper.bites.bit_6) value += qPow(2, 2);
    if(temper.bites.bit_5) value += qPow(2, 1);
    if(temper.bites.bit_4) value += qPow(2, 0);
    if(temper.bites.bit_3) value += qPow(2, -1);
    if(temper.bites.bit_2) value += qPow(2, -2);
    if(temper.bites.bit_1) value += qPow(2, -3);
    if(temper.bites.bit_0) value += qPow(2, -4);

    return (sign * value);
}
//--------------------------------------------------------------------------------
float DS18B20::convert_10bit(unsigned short temp_ds18b20)
{
    short temp;
    union TEMPERATURE temper;

    temp = temp_ds18b20 >> 2;

    temper.value = temp;

    int sign = 1;
    if(temper.bites.bit_9)
        sign = -1;

    if(temper.bites.bit_9)
        temp = (~temp + 1);
    temper.value = temp << 2;

    float value = 0;
    if(temper.bites.bit_8) value += qPow(2, 4);
    if(temper.bites.bit_7) value += qPow(2, 3);
    if(temper.bites.bit_6) value += qPow(2, 2);
    if(temper.bites.bit_5) value += qPow(2, 1);
    if(temper.bites.bit_4) value += qPow(2, 0);
    if(temper.bites.bit_3) value += qPow(2, -1);
    if(temper.bites.bit_2) value += qPow(2, -2);
    if(temper.bites.bit_1) value += qPow(2, -3);
    if(temper.bites.bit_0) value += qPow(2, -4);

    return (sign * value);
}
//--------------------------------------------------------------------------------
float DS18B20::convert_11bit(unsigned short temp_ds18b20)
{
    short temp;
    union TEMPERATURE temper;

    temp = temp_ds18b20 >> 1;

    temper.value = temp;

    int sign = 1;
    if(temper.bites.bit_10)
        sign = -1;

    if(temper.bites.bit_10)
        temp = (~temp + 1);
    temper.value = temp << 1;

    float value = 0;
    if(temper.bites.bit_9) value += qPow(2, 5);
    if(temper.bites.bit_8) value += qPow(2, 4);
    if(temper.bites.bit_7) value += qPow(2, 3);
    if(temper.bites.bit_6) value += qPow(2, 2);
    if(temper.bites.bit_5) value += qPow(2, 1);
    if(temper.bites.bit_4) value += qPow(2, 0);
    if(temper.bites.bit_3) value += qPow(2, -1);
    if(temper.bites.bit_2) value += qPow(2, -2);
    if(temper.bites.bit_1) value += qPow(2, -3);
    if(temper.bites.bit_0) value += qPow(2, -4);

    return (sign * value);
}
//--------------------------------------------------------------------------------
float DS18B20::convert_12bit(unsigned short temp_ds18b20)
{
    unsigned short temp;
    union TEMPERATURE temper;

    temp = temp_ds18b20;

    temper.value = temp;

    int sign = 1;
    if(temper.bites.bit_11)
        sign = -1;

    if(temper.bites.bit_11)
        temp = (~temp + 1);
    temper.value = temp;

    float value = 0;
    if(temper.bites.bit_10) value += qPow(2, 6);
    if(temper.bites.bit_9)  value += qPow(2, 5);
    if(temper.bites.bit_8)  value += qPow(2, 4);
    if(temper.bites.bit_7)  value += qPow(2, 3);
    if(temper.bites.bit_6)  value += qPow(2, 2);
    if(temper.bites.bit_5)  value += qPow(2, 1);
    if(temper.bites.bit_4)  value += qPow(2, 0);
    if(temper.bites.bit_3)  value += qPow(2, -1);
    if(temper.bites.bit_2)  value += qPow(2, -2);
    if(temper.bites.bit_1)  value += qPow(2, -3);
    if(temper.bites.bit_0)  value += qPow(2, -4);

    return (sign * value);
}
//--------------------------------------------------------------------------------
#if 1
void DS18B20::prepare_data(void)
{
    ft2232h->append_data('\x8A');  // Ensure disable clock divide by 5 for 60Mhz master clock
    ft2232h->append_data('\x97');  // Ensure turn off adaptive clocking
    ft2232h->append_data('\x8D');  // disable 3 phase data clock

    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7);
    ft2232h->append_data(SEARCH_ROM);

    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_IN);
    ft2232h->append_data(7);
}
#else
void DS18B20::prepare_data(void)
{
    ft2232h->append_data(MSB_RISING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7);
    ft2232h->append_data(SEARCH_ROM);

    ft2232h->append_data(0x87);

    ft2232h->append_data(MSB_RISING_EDGE_CLOCK_BIT_IN);
    ft2232h->append_data(7);
}
#endif
//--------------------------------------------------------------------------------
void DS18B20::prepare_data2(void)
{
    BYTE dir = 0x04;
    for(int n=0; n<4; n++)
    {
        ft2232h->append_data(SET_DATA_BITS_HI_BYTE);
        ft2232h->append_data(0);
        ft2232h->append_data(dir);
    }

    for(int n=0; n<4; n++)
    {
        ft2232h->append_data(SET_DATA_BITS_HI_BYTE);
        ft2232h->append_data(dir);
        ft2232h->append_data(dir);
    }

    for(int n=0; n<8; n++)
    {
        ft2232h->append_data(GET_DATA_BITS_HI_BYTE);
    }
}
//--------------------------------------------------------------------------------
void DS18B20::test(void)
{
    emit info("test DS18B20");

#if 0
    emit debug(QString("+125     value %1").arg(convert_12bit(0x07D0)));
    emit debug(QString("+85      value %1").arg(convert_12bit(0x0550)));
    emit debug(QString("+25.0625 value %1").arg(convert_12bit(0x0191)));
    emit debug(QString("+10.125  value %1").arg(convert_12bit(0x00A2)));
    emit debug(QString("+0.5     value %1").arg(convert_12bit(0x0008)));
    emit debug(QString("0        value %1").arg(convert_12bit(0x0000)));
    emit debug(QString("-0.5     value %1").arg(convert_12bit(0xFFF8)));
    emit debug(QString("-10.125  value %1").arg(convert_12bit(0xFF5E)));
    emit debug(QString("-25.0625 value %1").arg(convert_12bit(0xFE6F)));
    emit debug(QString("-55      value %1").arg(convert_12bit(0xFC90)));
#endif

    FT_STATUS ftStatus = FT_OK;

    ftStatus = ft2232h->open(0);
    if(ftStatus != FT_OK)
    {
        return;
    }

    //----
    ft2232h->clear_data();
    prepare_data();
    ftStatus = ft2232h->write_data();
    if(ftStatus != FT_OK)
    {
        return;
    }

    DWORD dwNumInputBuffer = 0;
    ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);
    if(ftStatus != FT_OK)
    {
        return;
    }
    emit info(QString("return %1 bytes").arg(dwNumInputBuffer));

    //----

    ft2232h->close();

    emit info("OK");
}
//--------------------------------------------------------------------------------
#if 0
union TEMPERATURE temp;

temp.value = 0;
temp.bites.bit_f = 1;
qDebug() << temp.value;
#endif
