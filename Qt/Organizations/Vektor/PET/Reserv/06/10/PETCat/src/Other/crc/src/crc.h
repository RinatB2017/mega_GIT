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
#include <stdint.h>
#include <QObject>
//--------------------------------------------------------------------------------
/* CRC-32C (iSCSI) polynomial in reversed bit order. */
#define POLY 0x82f63b78

/* CRC-32 (Ethernet, ZIP, etc.) polynomial in reversed bit order. */
/* #define POLY 0xedb88320 */
//--------------------------------------------------------------------------------
#define htons(n) (unsigned short)((((unsigned short) (n)) << 8) | (((unsigned short) (n)) >> 8))
#define htonl(n) (unsigned int)((((unsigned int) (n)) << 16) | (((unsigned int) (n)) >> 16))
//--------------------------------------------------------------------------------
class CRC
{
public:
    static uint8_t  crc8(uint8_t *pcBlock, uint8_t len);
    static uint16_t crc16(uint8_t *pcBlock, uint8_t len);
    static uint16_t modbus_crc16(uint8_t *pcBlock, uint16_t len);
    static uint16_t multimeter_crc16(uint8_t *pcBlock, uint16_t len);
    static uint16_t powersupply_crc16(uint8_t *pcBlock, uint16_t len);
#ifdef USE_CRC16
    static uint16_t pet_crc16(uint8_t *pcBlock, uint16_t len);
#endif
    static uint16_t java_crc16(uint8_t *pcBlock, uint8_t len);
    
#if 0
    static uint32_t crc32c(uint32_t crc, const unsigned char *buf, size_t len);
#else
    static uint32_t pet_crc32(void *data, size_t len);
#endif
};
//--------------------------------------------------------------------------------
#endif
