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
#ifndef UNION_HPP
#define UNION_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
union U_BYTE
{
    uint8_t value;
    struct
    {
        uint8_t bit0:1;
        uint8_t bit1:1;
        uint8_t bit2:1;
        uint8_t bit3:1;
        uint8_t bit4:1;
        uint8_t bit5:1;
        uint8_t bit6:1;
        uint8_t bit7:1;
    } bites;
};

union U_BYTE_DECADES
{
    uint8_t value;
    struct
    {
        uint8_t lo:4;
        uint8_t hi:4;
    } decades;
};


union U_SHORT
{
    unsigned short value;
    struct
    {
        uint8_t lo;
        uint8_t hi;
    } bytes;
};
//--------------------------------------------------------------------------------
#endif
