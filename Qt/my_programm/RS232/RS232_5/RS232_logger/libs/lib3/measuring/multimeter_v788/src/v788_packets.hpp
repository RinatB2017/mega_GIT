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
#ifndef V788_PACKETS_HPP
#define V788_PACKETS_HPP
//--------------------------------------------------------------------------------
;
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
#define V788_CMD_0x41     0x41
#define V788_CMD_0x41_TEXT   tr("В7-88 функция 0x41: Сброс в исходное состояние.")

#define V788_CMD_0x42     0x42
#define V788_CMD_0x42_TEXT   tr("В7-88 функция 0x42: Установка скорости обмена.")

#define V788_CMD_0x45     0x45
#define V788_CMD_0x45_TEXT   tr("В7-88 функция 0x45: Инициализация вторичной станции.")

#define V788_CMD_0x46     0x46
#define V788_CMD_0x46_TEXT   tr("В7-88 функция 0x46: Запрос идентификатора.")

#define V788_CMD_0x47     0x47
#define V788_CMD_0x47_TEXT   tr("В7-88 функция 0x47: Запрос состояния.")

#define V788_CMD_0x48     0x48
#define V788_CMD_0x48_TEXT   tr("В7-88 функция 0x48: Переадресация.")

#define V788_CMD_0x53     0x53
#define V788_CMD_0x53_TEXT   tr("В7-88 функция 0x53: Управление прибором.")
//--------------------------------------------------------------------------------
typedef struct
{
    unsigned char  address;
    unsigned char  cmd;
    unsigned char  count_data;
} V788_HEADER;
//--------------------------------------------------------------------------------
struct CMD_V788_ERROR_ANSWER
{
    V788_HEADER header;
    unsigned char error;
    unsigned short crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V788_0x41_QUESTION
{
    V788_HEADER header;
    unsigned short crc16;
};

struct CMD_V788_0x41_ANSWER
{
    V788_HEADER header;
    unsigned short crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V788_0x42_QUESTION
{
    V788_HEADER header;
    unsigned char  baud;
    unsigned char  time;
    unsigned short crc16;
};

struct CMD_V788_0x42_ANSWER
{
    V788_HEADER header;
    unsigned short crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V788_0x45_QUESTION
{
    V788_HEADER header;
    unsigned short crc16;
};

struct CMD_V788_0x45_ANSWER
{
    V788_HEADER header;
    unsigned short crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V788_0x46_QUESTION
{
    V788_HEADER header;
    unsigned short crc16;
};

struct CMD_V788_0x46_ANSWER
{
    V788_HEADER header;
    unsigned char  type;
    unsigned char  year;
    unsigned char  month;
    unsigned char  serno_lo;
    unsigned char  serno_hi;
    unsigned char  baud;
    unsigned char  time_waiting;
    unsigned char  address_station;
    unsigned char  byte_test;
    unsigned short crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V788_0x47_QUESTION
{
    V788_HEADER header;
    unsigned short crc16;
};

struct CMD_V788_0x47_ANSWER
{
    V788_HEADER header;
    unsigned char  state;
    unsigned char  error;
    signed int     value;
    unsigned short crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V788_0x48_QUESTION
{
    V788_HEADER header;
    unsigned char  data_0;
    unsigned short crc16;
};

struct CMD_V788_0x48_ANSWER
{
    V788_HEADER header;
    unsigned char  address_station;
    unsigned short crc16;
};
//--------------------------------------------------------------------------------
struct CMD_V788_0x53_QUESTION
{
    V788_HEADER header;
    unsigned char  data_0;
    unsigned short crc16;
};

struct CMD_V788_0x53_ANSWER
{
    V788_HEADER header;
    unsigned char  data_0;
    unsigned short crc16;
};

struct CMD_V788_0x53_ANSWER_ERROR
{
    V788_HEADER header;
    unsigned char error;
    unsigned short crc16;
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
