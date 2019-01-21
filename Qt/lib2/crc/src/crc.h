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
#ifndef CRC_H
#define CRC_H
//--------------------------------------------------------------------------------
#include <QtGlobal>
//--------------------------------------------------------------------------------
#ifdef Q_OS_WIN
#   include <winsock2.h>
#   include <windows.h>
#endif
//--------------------------------------------------------------------------------
#include <QObject>
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   define htons(n) (uint16_t)((((uint16_t) (n)) << 8) | (((uint16_t) (n)) >> 8))
#   define htonl(n) (uint32_t)((((uint32_t) (n)) << 16) | (((uint32_t) (n)) >> 16))
#endif
//--------------------------------------------------------------------------------
class CRC
{
public:
    static uint8_t  pelco_crc8(uint8_t *pcBlock, uint8_t len);

    static uint8_t  crc8(uint8_t *pcBlock, uint8_t len);
    static uint16_t crc16(uint8_t *pcBlock, uint16_t len);
    static uint16_t modbus_crc16(uint8_t *pcBlock, uint16_t len);
    static uint16_t multimeter_crc16(uint8_t *pcBlock, uint16_t len);
    static uint16_t powersupply_crc16(uint8_t *pcBlock, uint16_t len);
#ifdef USE_CRC16
    static uint16_t pet_crc16(uint8_t *pcBlock, uint16_t len);
#endif
    static uint16_t java_crc16(uint8_t *pcBlock, uint8_t len);
};
//--------------------------------------------------------------------------------
#endif
