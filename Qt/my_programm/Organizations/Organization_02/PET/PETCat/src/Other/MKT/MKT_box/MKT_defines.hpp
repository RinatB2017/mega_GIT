/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
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
#ifndef MKT_DEFINES_HPP
#define MKT_DEFINES_HPP
//--------------------------------------------------------------------------------
//#include <QtGlobal>
//--------------------------------------------------------------------------------
#define DEFAULT_MODBUS_TIMEOUT_SEC  1
//--------------------------------------------------------------------------------
#define MIN_MODBUS_ADDRESS  1
#define MAX_MODBUS_ADDRESS  247
//--------------------------------------------------------------------------------
#define MIN_INTERVAL    1
#define MAX_INTERVAL    60
//--------------------------------------------------------------------------------
#define MIN_SENSORS     1
#define MAX_SENSORS     50
//--------------------------------------------------------------------------------
#define MIN_WIDTH_WINDOW    1
#define MAX_WIDTH_WINDOW    10
//--------------------------------------------------------------------------------
#define MIN_TEMPERATURE     -100
#define MAX_TEMPERATURE     100
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   define DEFAULT_SLAVE_NAME      "/dev/ttyUSB0"
#endif
#ifdef Q_OS_WIN
#   define DEFAULT_SLAVE_NAME      "COM1"
#endif
//--------------------------------------------------------------------------------
#define DEFAULT_SLAVE_ADDRESS   1
#define DEFAULT_WIDTH_WINDOW    2
#define DEFAULT_NUM_SENSORS     1
#define DEFAULT_MIN_TEMPERATURE 0
#define DEFAULT_MAX_TEMPERATURE 50
//--------------------------------------------------------------------------------
#define ADDRESS_STATUS          0
#define ADDRESS_NUM_SENSORS     1
#define ADDRESS_WIDTH_WINDOW    2
#define ADDRESS_RESET           3
#define ADDRESS_VER_FIRMVARE    4
//--------------------------------------------------------------------------------
#define ERROR_VALUE_SMALL   "значение слишком маленькое"
#define ERROR_VALUE_LARGE   "значение слишком большое"
//--------------------------------------------------------------------------------
#endif
