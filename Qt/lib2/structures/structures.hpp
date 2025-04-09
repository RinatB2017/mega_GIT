/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
union U_DAC_8 {
    uint8_t value;
    struct {
        unsigned LOW_0_6525:1;
        unsigned LOW_0_125:1;
        unsigned LOW_0_25:1;
        unsigned LOW_0_5:1;
        unsigned HIGH_0_6525:1;
        unsigned HIGH_0_125:1;
        unsigned HIGH_0_25:1;
        unsigned HIGH_0_5:1;
    } bites;
};

union U_BYTE {
    uint8_t value;
    struct {
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

union U_8 {
    uint8_t value;
    struct {
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

union U_16 {
    uint16_t value;
    struct {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
        unsigned bit8:1;
        unsigned bit9:1;
        unsigned bit10:1;
        unsigned bit11:1;
        unsigned bit12:1;
        unsigned bit13:1;
        unsigned bit14:1;
        unsigned bit15:1;
    } bites;
};

union U_32 {
    uint32_t value;
    struct {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
        unsigned bit8:1;
        unsigned bit9:1;
        unsigned bit10:1;
        unsigned bit11:1;
        unsigned bit12:1;
        unsigned bit13:1;
        unsigned bit14:1;
        unsigned bit15:1;
        unsigned bit16:1;
        unsigned bit17:1;
        unsigned bit18:1;
        unsigned bit19:1;
        unsigned bit20:1;
        unsigned bit21:1;
        unsigned bit22:1;
        unsigned bit23:1;
        unsigned bit24:1;
        unsigned bit25:1;
        unsigned bit26:1;
        unsigned bit27:1;
        unsigned bit28:1;
        unsigned bit29:1;
        unsigned bit30:1;
        unsigned bit31:1;
    } bites;
};
union U_32_bytes {
    uint32_t value;
    struct {
        uint8_t D4;
        uint8_t D5;
        uint8_t D6;
        uint8_t D7;
    } bytes;
};
//--------------------------------------------------------------------------------
#endif
