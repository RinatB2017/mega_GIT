/*********************************************************************************
**                                                                              **
**     Copyright (C) 2022                                                       **
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
#ifndef V764_2_PACKETS_HPP
#define V764_2_PACKETS_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
;
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
//#define MIN_ADDRESS 1
//#define MAX_ADDRESS 255

#define V764_2_CMD_0x41     0x41
#define V764_2_CMD_0x41_TEXT   tr("В7-64/2 функция 0x41: Сброс в исходное состояние.")

#define V764_2_CMD_0x42     0x42
#define V764_2_CMD_0x42_TEXT   tr("В7-64/2 функция 0x42: Установка скорости обмена.")

#define V764_2_CMD_0x45     0x45
#define V764_2_CMD_0x45_TEXT   tr("В7-64/2 функция 0x45: Инициализация вторичной станции.")

#define V764_2_CMD_0x46     0x46
#define V764_2_CMD_0x46_TEXT   tr("В7-64/2 функция 0x46: Запрос идентификатора.")

#define V764_2_CMD_0x47     0x47
#define V764_2_CMD_0x47_TEXT   tr("В7-64/2 функция 0x47: Запрос состояния.")

#define V764_2_CMD_0x48     0x48
#define V764_2_CMD_0x48_TEXT   tr("В7-64/2 функция 0x48: Переадресация.")

#define V764_2_CMD_0x53     0x53
#define V764_2_CMD_0x53_TEXT   tr("В7-64/2 функция 0x53: Управление прибором.")
//--------------------------------------------------------------------------------
typedef struct
{
    uint8_t  address;
    uint8_t  cmd;
    uint8_t  count_data;
} V7642_HEADER;
//--------------------------------------------------------------------------------
struct CMD_V764_2_ERROR_ANSWER
{
    V7642_HEADER header;
    uint8_t error;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V764_2_0x41_QUESTION
{
    V7642_HEADER header;
    uint16_t crc16;
};

struct CMD_V764_2_0x41_ANSWER
{
    V7642_HEADER header;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V764_2_0x42_QUESTION
{
    V7642_HEADER header;
    uint8_t  baud;
    uint8_t  time;
    uint16_t crc16;
};

struct CMD_V764_2_0x42_ANSWER
{
    V7642_HEADER header;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V764_2_0x45_QUESTION
{
    V7642_HEADER header;
    uint16_t crc16;
};

struct CMD_V764_2_0x45_ANSWER
{
    V7642_HEADER header;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V764_2_0x46_QUESTION
{
    V7642_HEADER header;
    uint16_t crc16;
};

struct CMD_V764_2_0x46_ANSWER
{
    V7642_HEADER header;
    uint8_t  type;
    uint8_t  year;
    uint8_t  month;
    uint8_t  serno_lo;
    uint8_t  serno_hi;
    uint8_t  baud;
    uint8_t  time_waiting;
    uint8_t  address_station;
    uint8_t  byte_test;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V764_2_0x47_QUESTION
{
    V7642_HEADER header;
    uint16_t crc16;
};

struct CMD_V764_2_0x47_ANSWER
{
    V7642_HEADER header;
    uint8_t  state;
    uint8_t  error;
    signed int     value;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V764_2_0x48_QUESTION
{
    V7642_HEADER header;
    uint8_t  data_0;
    uint16_t crc16;
};

struct CMD_V764_2_0x48_ANSWER
{
    V7642_HEADER header;
    uint8_t  address_station;
    uint16_t crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V764_2_0x53_QUESTION
{
    V7642_HEADER header;
    uint8_t  data_0;
    uint16_t crc16;
};

struct CMD_V764_2_0x53_ANSWER
{
    V7642_HEADER header;
    uint8_t  data_0;
    uint16_t crc16;
};

struct CMD_V764_2_0x53_ANSWER_ERROR
{
    V7642_HEADER header;
    uint8_t error;
    uint16_t crc16;
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
