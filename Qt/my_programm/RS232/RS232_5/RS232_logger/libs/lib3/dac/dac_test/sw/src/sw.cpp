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
#include "sw.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#define OLD
//--------------------------------------------------------------------------------
SW::SW()
{

}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D4_1(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 255;
#else
    dac.value = 254;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(1);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D4_2(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 224;
#else
    dac.value = 240;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(1);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D4_3(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 192;
#else
    dac.value = 208;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(1);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D4_4(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 160;
#else
    dac.value = 176;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(1);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D4_1(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 255;
#else
    dac.value = 254;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(1);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D4_2(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 224;
#else
    dac.value = 240;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(1);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D4_3(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 192;
#else
    dac.value = 208;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(1);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D4_4(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 160;
#else
    dac.value = 176;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(1);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D5_1(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 128;
#else
    dac.value = 144;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(1);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D5_2(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 96;
#else
    dac.value = 112;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(1);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D5_3(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 64;
#else
    dac.value = 80;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(1);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D5_4(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 32;
#else
    dac.value = 48;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(1);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D5_1(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 128;
#else
    dac.value = 144;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(1);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D5_2(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 96;
#else
    dac.value = 112;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(1);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D5_3(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 64;
#else
    dac.value = 80;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(1);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D5_4(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(0);
    K3(1);

    //---
    U_8 dac;
#ifdef OLD
    dac.value = 32;
#else
    dac.value = 48;
#endif

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(0);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(1);

    return sw;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D15_1(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(0);
    HIGH_0_25(1);
    HIGH_0_5(0);

    LOW_0_6525(1);
    LOW_0_125(1);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D15_2(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(0);
    HIGH_0_25(0);
    HIGH_0_5(1);

    LOW_0_6525(1);
    LOW_0_125(1);
    LOW_0_25(1);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D15_3(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(1);
    HIGH_0_125(0);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(0);
    LOW_0_125(0);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D15_4(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(1);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(1);
    LOW_0_125(0);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D15_1(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(0);
    HIGH_0_25(1);
    HIGH_0_5(0);

    LOW_0_6525(1);
    LOW_0_125(1);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D15_2(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(0);
    HIGH_0_25(0);
    HIGH_0_5(1);

    LOW_0_6525(1);
    LOW_0_125(1);
    LOW_0_25(1);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D15_3(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(1);
    HIGH_0_125(0);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(0);
    LOW_0_125(0);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D15_4(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(1);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(1);
    LOW_0_125(0);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D16_1(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(0);
    HIGH_0_25(1);
    HIGH_0_5(0);

    LOW_0_6525(1);
    LOW_0_125(1);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D16_2(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(0);
    HIGH_0_25(0);
    HIGH_0_5(1);

    LOW_0_6525(1);
    LOW_0_125(1);
    LOW_0_25(1);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D16_3(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(1);
    HIGH_0_125(0);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(0);
    LOW_0_125(0);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_plus_D16_4(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(0);
    D16_REF(1);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(1);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(1);
    LOW_0_125(0);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D16_1(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(1);
    D16_REF(0);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(1);
    HIGH_0_125(1);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(0);
    LOW_0_125(0);
    LOW_0_25(1);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D16_2(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(1);
    D16_REF(0);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(1);
    HIGH_0_125(1);
    HIGH_0_25(1);
    HIGH_0_5(0);

    LOW_0_6525(0);
    LOW_0_125(0);
    LOW_0_25(0);
    LOW_0_5(1);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D16_3(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(1);
    D16_REF(0);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(0);
    HIGH_0_125(0);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(1);
    LOW_0_125(0);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_minus_D16_4(void)
{
    SWITCH sw;
    sw.value = 0;

    D15_REF(1);
    D16_REF(0);

    K1(1);
    K2(0);
    K3(0);

    //---
    HIGH_0_6525(1);
    HIGH_0_125(0);
    HIGH_0_25(0);
    HIGH_0_5(0);

    LOW_0_6525(0);
    LOW_0_125(1);
    LOW_0_25(0);
    LOW_0_5(0);
    //---

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_positive_voltage(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(1);
    D16_REF(0);

    R2_REF_DC(0);
    DAC_LOW_REF(0);

    K1(0);
    K2(1);
    K3(1);

    //---
    U_8 dac;
    dac.value = 255;

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(1);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
SWITCH SW::get_weighing_negative_voltage(void)
{
    SWITCH sw;
    sw.value = 0;

    R2_IN(1);
    R2_OUT(1);

    D15_REF(0);
    D16_REF(1);

    R2_REF_DC(1);
    DAC_LOW_REF(1);

    K1(0);
    K2(1);
    K3(1);

    //---
    U_8 dac;
    dac.value = 255;

    HIGH_0_6525(dac.bites.HIGH_0_6525);
    HIGH_0_125(dac.bites.HIGH_0_125);
    HIGH_0_25(dac.bites.HIGH_0_25);
    HIGH_0_5(dac.bites.HIGH_0_5);

    LOW_0_6525(dac.bites.LOW_0_6525);
    LOW_0_125(dac.bites.LOW_0_125);
    LOW_0_25(dac.bites.LOW_0_25);
    LOW_0_5(dac.bites.LOW_0_5);
    //---

    LADDER_6_4(1);
    LADDER_5_6(0);
    LADDER_4_8(0);
    LADDER_4_0(0);
    LADDER_3_2(0);
    LADDER_2_4(0);
    LADDER_1_6(0);
    LADDER_0_8(0);

    return sw;
}
//--------------------------------------------------------------------------------
