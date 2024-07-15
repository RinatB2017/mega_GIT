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
#ifndef CRC_H
#define CRC_H
//--------------------------------------------------------------------------------
#include <QtGlobal>
#ifdef Q_OS_WIN
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
// для винды не надо включать winsock2.h и надо добавить в проект
// DEFINES += WIN32_LEAN_AND_MEAN
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#   define htons(n) static_cast<uint16_t>((static_cast<uint16_t>(n) << 8)  | (static_cast<uint16_t>(n) >> 8))
#   define htonl(n) static_cast<uint32_t>((static_cast<uint32_t>(n) << 16) | (static_cast<uint32_t>(n) >> 16))
#endif
#ifdef Q_OS_WIN
#   define htons(n) (uint16_t)((((uint16_t) (n)) << 8) | (((uint16_t) (n)) >> 8))
#   define htonl(n) (uint32_t)((((uint32_t) (n)) << 16) | (((uint32_t) (n)) >> 16))
#endif
//--------------------------------------------------------------------------------
class CRC
{
public:
    static uint8_t  pelco_crc8(uint8_t *pcBlock, uint8_t len);

    static uint8_t  crc8(uint8_t *pcBlock, uint8_t len);
    static int8_t   crc8(int8_t *buf, int offset, int length);
    static uint16_t crc16(uint8_t *pcBlock, uint16_t len);
    static uint32_t crc32(const char *buf, size_t len);
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
