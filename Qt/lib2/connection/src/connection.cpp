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
#include <QObject>
//--------------------------------------------------------------------------------
#include "connection.hpp"
//--------------------------------------------------------------------------------
double   Connection::value_double;
float    Connection::value_float;
uint8_t  Connection::value_uint8;
int8_t   Connection::value_int8;
uint16_t Connection::value_uint16;
int16_t  Connection::value_int16;
uint32_t Connection::value_uint32;
int32_t  Connection::value_int32;
int      Connection::value_int;
//--------------------------------------------------------------------------------
Connection::Connection(QObject *parent) : QObject(parent)
{
    value_double = 0;
    value_float  = 0;
    value_uint8  = 0;
    value_int8   = 0;
    value_uint16 = 0;
    value_int16  = 0;
    value_uint32 = 0;
    value_int32  = 0;
}
//--------------------------------------------------------------------------------
void Connection::set_value(double value)
{
    value_double = value;
}
//--------------------------------------------------------------------------------
void Connection::set_value(float value)
{
    value_float = value;
}
//--------------------------------------------------------------------------------
void Connection::set_value(uint8_t value)
{
    value_uint8 = value;
}
//--------------------------------------------------------------------------------
void Connection::set_value(int8_t value)
{
    value_int8 = value;
}
//--------------------------------------------------------------------------------
void Connection::set_value(uint16_t value)
{
    value_uint16 = value;
}
//--------------------------------------------------------------------------------
void Connection::set_value(int16_t value)
{
    value_int16 = value;
}
//--------------------------------------------------------------------------------
void Connection::set_value(uint32_t value)
{
    value_uint32 = value;
}
//--------------------------------------------------------------------------------
void Connection::set_value(int32_t value)
{
    value_int32 = value;
}
//--------------------------------------------------------------------------------
double Connection::get_double_value(void)
{
    return value_double;
}
//--------------------------------------------------------------------------------
float Connection::get_float_value(void)
{
    return value_float;
}
//--------------------------------------------------------------------------------
uint8_t Connection::get_uint8_value(void)
{
    return value_uint8;
}
//--------------------------------------------------------------------------------
int8_t Connection::get_int8_value(void)
{
    return value_int8;
}
//--------------------------------------------------------------------------------
uint16_t Connection::get_uint16_value(void)
{
    return value_uint16;
}
//--------------------------------------------------------------------------------
int16_t Connection::get_int16_value(void)
{
    return value_int16;
}
//--------------------------------------------------------------------------------
uint32_t Connection::get_uint32_value(void)
{
    return value_uint32;
}
//--------------------------------------------------------------------------------
int32_t Connection::get_int32_value(void)
{
    return value_int32;
}
//--------------------------------------------------------------------------------
void Connection::set_int_value(int value)
{
    value_int = value;
}
//--------------------------------------------------------------------------------
int Connection::get_int_value(void)
{
    return value_int;
}
//--------------------------------------------------------------------------------
