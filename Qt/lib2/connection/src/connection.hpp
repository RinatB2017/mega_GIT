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
#ifndef CONNECTION_HPP
#define CONNECTION_HPP
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   include <stdint.h>
#endif
#ifdef Q_OS_WIN
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
class Connection : public QObject
{
    Q_OBJECT
public:
    static void set_value(double value);
    static void set_value(float value);
    static void set_value(uint8_t value);
    static void set_value(int8_t value);
    static void set_value(uint16_t value);
    static void set_value(int16_t value);
    static void set_value(uint32_t value);
    static void set_value(int32_t value);

    static double   get_double_value(void);
    static float    get_float_value(void);
    static uint8_t  get_uint8_value(void);
    static int8_t   get_int8_value(void);
    static uint16_t get_uint16_value(void);
    static int16_t  get_int16_value(void);
    static uint32_t get_uint32_value(void);
    static int32_t  get_int32_value(void);

    static void set_int_value(int value);
    static int  get_int_value(void);

private:
    explicit Connection(QObject *parent = nullptr);

    static double   value_double;
    static float    value_float;
    static uint8_t  value_uint8;
    static int8_t   value_int8;
    static uint16_t value_uint16;
    static int16_t  value_int16;
    static uint32_t value_uint32;
    static int32_t  value_int32;

    static int      value_int;
};
//--------------------------------------------------------------------------------
#endif
