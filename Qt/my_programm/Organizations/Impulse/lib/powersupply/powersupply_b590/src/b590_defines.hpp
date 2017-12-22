/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#ifndef B590_DEFINES_HPP
#define B590_DEFINES_HPP
//--------------------------------------------------------------------------------
#define MAX_CALIBRATION_POINTS_B590_U    61
#define MAX_CALIBRATION_POINTS_B590_I    51

// может быть отрицательным, если есть реверс
#define MIN_VOLTAGE_B590 -60000
#define MAX_VOLTAGE_B590 60000

#define MIN_CURRENT_B590 0
#define MAX_CURRENT_B590 50000

#define MIN_PROFILE_B590 0
#define MAX_PROFILE_B590 9

#define MIN_POINT_B590   0
#define MAX_POINT_B590   30

#define MIN_REPEAT_B590  1
#define MAX_REPEAT_B590  250

#define MIN_ADDRESS_B590 1
#define MAX_ADDRESS_B590 251

#define MIN_DELAY_B590   1000
#define MAX_DELAY_B590   0xFFFF

#define MIN_ID_B590      1
#define MAX_ID_B590      0xFF

#define MIN_TIME_B590    1
#define MAX_TIME_B590    0xFFFF

#define MIN_DATA_B590    1
#define MAX_DATA_B590    0xFFFF

#define MIN_SPEED_B590   1
#define MAX_SPEED_B590   0xFFFF

#define MIN_PWM_B590     1
#define MAX_PWM_B590     0xFFFF

#define MIN_COUNTER_B590 1
#define MAX_COUNTER_B590 0xFF

#define MIN_CONF_ADC_B590    1
#define MAX_CONF_ADC_B590    0xFF

#define MIN_CODE_BUTTON_B590 1
#define MAX_CODE_BUTTON_B590 16

#define MIN_CMD_B590 1
#define MAX_CMD_B590 0xFF

#define MIN_NUMBER_STRING_B590   1
#define MAX_NUMBER_STRING_B590   6
//--------------------------------------------------------------------------------
#define MAX_B590_POWER         750000000UL // 750000000UL
#define MAX_B590_VALUE_U       MAX_VOLTAGE_B590
#define MAX_B590_VALUE_I       MAX_CURRENT_B590

#define MIN_B590_VALUE_U 1
#define MIN_B590_VALUE_I 1

#define MINIMUM_B590_I   10

#define CALIBRATION_POINTS_U    4
#define CALIBRATION_POINTS_I    13
// Число калибровочных точек для ЦАП
#define COUNT_B590_CALIB_POINTS_DAC_U MAX_CALIBRATION_POINTS_B590_U
#define COUNT_B590_CALIB_POINTS_DAC_I MAX_CALIBRATION_POINTS_B590_I

#define POINT_B590_U_0 10       // 10мВ нужно для корректного измерения (не требует метрологии)
#define POINT_B590_U_1 1000     // Нижняя граница калибровки 1В
#define POINT_B590_U_2 60000	// Верхняя граница калибровки 59В
#define POINT_B590_U_3 65000    // 65В нужно для корректного измерения (не требует метрологии)

#define POINT_B590_I_0 10       // 10мА нужно для корректного измерения (не требует метрологии)
#define POINT_B590_I_1 50       // Нижняя граница калибровки 0,05А
#define POINT_B590_I_2 5000     // Вторая точка
#define POINT_B590_I_3 25000    // Третья точка
#define POINT_B590_I_4 50000	// Верхняя граница калибровки 48А
#define POINT_B590_I_5 64000    // 64А нужно для корректного измерения (не требует метрологии)
//--------------------------------------------------------------------------------
#define MAGIC_B590_U    1000
#define MAGIC_B590_I    1000
//--------------------------------------------------------------------------------
//Функция записи года,месяца выпуска и серийного номера прибора
#define TYPE_WRITE_FACTORY_YEAR_MONTH_SERIAL 0x0F
//Функция обнуления счётчика моточасов
#define TYPE_RESET_MOTOMINUTES 0x10
//Функция Создание заводских калибровок
#define TYPE_SAVE_FACTORY_CALIB 0x11
//--------------------------------------------------------------------------------
#endif
