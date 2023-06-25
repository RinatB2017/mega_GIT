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
#ifndef SW_HPP
#define SW_HPP
//--------------------------------------------------------------------------------
#include "packets.h"
//--------------------------------------------------------------------------------
class SW
{
public:
    SW();

    //new
    static SWITCH get_weighing_plus_D4_1(void);
    static SWITCH get_weighing_plus_D4_2(void);
    static SWITCH get_weighing_plus_D4_3(void);
    static SWITCH get_weighing_plus_D4_4(void);

    static SWITCH get_weighing_minus_D4_1(void);
    static SWITCH get_weighing_minus_D4_2(void);
    static SWITCH get_weighing_minus_D4_3(void);
    static SWITCH get_weighing_minus_D4_4(void);
    //---

    static SWITCH get_weighing_plus_D5_1(void);
    static SWITCH get_weighing_plus_D5_2(void);
    static SWITCH get_weighing_plus_D5_3(void);
    static SWITCH get_weighing_plus_D5_4(void);

    static SWITCH get_weighing_minus_D5_1(void);
    static SWITCH get_weighing_minus_D5_2(void);
    static SWITCH get_weighing_minus_D5_3(void);
    static SWITCH get_weighing_minus_D5_4(void);
    //---

    static SWITCH get_weighing_plus_D15_1(void);
    static SWITCH get_weighing_plus_D15_2(void);
    static SWITCH get_weighing_plus_D15_3(void);
    static SWITCH get_weighing_plus_D15_4(void);

    static SWITCH get_weighing_minus_D15_1(void);
    static SWITCH get_weighing_minus_D15_2(void);
    static SWITCH get_weighing_minus_D15_3(void);
    static SWITCH get_weighing_minus_D15_4(void);
    //---

    static SWITCH get_weighing_plus_D16_1(void);
    static SWITCH get_weighing_plus_D16_2(void);
    static SWITCH get_weighing_plus_D16_3(void);
    static SWITCH get_weighing_plus_D16_4(void);

    static SWITCH get_weighing_minus_D16_1(void);
    static SWITCH get_weighing_minus_D16_2(void);
    static SWITCH get_weighing_minus_D16_3(void);
    static SWITCH get_weighing_minus_D16_4(void);

    static SWITCH get_weighing_positive_voltage(void);
    static SWITCH get_weighing_negative_voltage(void);
    //---
};
//--------------------------------------------------------------------------------
#endif // SW_HPP
