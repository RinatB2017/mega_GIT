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
#include "ft2232h.hpp"
#include "bitbang.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
union B_BYTE
{
    unsigned char value;
    struct
    {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
    } bites;
};
//--------------------------------------------------------------------------------
BitBang::BitBang(I2C_Freq freq,
                 QWidget *parent) :
    MyWidget(parent),
    ft2232h(0)
{
    ft2232h = new FT2232H(freq, parent);

    open();
}
//--------------------------------------------------------------------------------
BitBang::~BitBang()
{
    close();
    ft2232h->close();
    ft2232h->deleteLater();
}
//--------------------------------------------------------------------------------
void BitBang::power_on(void)
{
    B_BYTE lo_byte;
    B_BYTE hi_byte;

    lo_byte.value = 0;
    lo_byte.bites.bit0 = 1;
    lo_byte.bites.bit1 = 1;
    lo_byte.bites.bit2 = 1;
    lo_byte.bites.bit3 = 1;

    hi_byte.value = 0;
    hi_byte.bites.bit0 = 1;
    hi_byte.bites.bit1 = 1;
    hi_byte.bites.bit2 = 1;

    ft2232h->clear_data();

    ft2232h->append_data(0x80);
    ft2232h->append_data(lo_byte.value);
    ft2232h->append_data(0x0F);

    ft2232h->append_data(0x82);
    ft2232h->append_data(hi_byte.value);
    ft2232h->append_data(0x07);

    if(ft2232h->write_data())
    {
        emit info("power_on OK!");
    }
}
//--------------------------------------------------------------------------------
void BitBang::power_off(void)
{
    ft2232h->clear_data();

    ft2232h->append_data(0x80);
    ft2232h->append_data(0);
    ft2232h->append_data(0x07);

    ft2232h->append_data(0x82);
    ft2232h->append_data(0);
    ft2232h->append_data(0x07);

    if(ft2232h->write_data())
    {
        emit info("power_off OK!");
    }
}
//--------------------------------------------------------------------------------
bool BitBang::open(void)
{
    return ft2232h->open(1);
}
//--------------------------------------------------------------------------------
void BitBang::close(void)
{
    ft2232h->close();
}
//--------------------------------------------------------------------------------
