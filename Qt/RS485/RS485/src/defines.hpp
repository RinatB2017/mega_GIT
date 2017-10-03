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
#ifndef DEFINES_HPP
#define DEFINES_HPP
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
#define ORGNAME "Work"
#define APPNAME "RS485"
//--------------------------------------------------------------------------------
#define ICON_PROGRAMM ":/mainwindow/computer.png"
//--------------------------------------------------------------------------------
#include "packet.hpp"
//--------------------------------------------------------------------------------
#if 0
#pragma pack (push, 1)

union CMD_QUESTION
{
    struct BODY{
        uint16_t prefix_16;                 // префикс
        uint16_t len_16;                    // длина пакета
        uint8_t  addr_8;                    // адрес модуля
        uint8_t  reserv_8;                  // резерв
        uint8_t  cmd_8;                     // команда
        uint8_t  reserv_2_8;                // резерв
        uint16_t time_interval_16;          // интервал дворника
        uint16_t reserv_3_16;               // резерв
        uint32_t time_washout_32;           // время помывки
        uint32_t time_pause_washout_32;     // время между помывками
        uint32_t addr_cam_32;               // адрес камеры
        uint32_t preset_washout_32;         // пресет помывки
        uint32_t time_preset_washout_32;    // времен помывки
    } body;
    unsigned char buf[sizeof(BODY)];
};

#pragma pack(pop)
#endif
//--------------------------------------------------------------------------------
#endif
