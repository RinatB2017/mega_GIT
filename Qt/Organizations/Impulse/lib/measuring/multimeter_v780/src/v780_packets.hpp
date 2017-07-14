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
#ifndef PACKETS_V780_HPP
#define PACKETS_V780_HPP
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
#define V780_CMD_0x41     0x41
#define V780_CMD_0x41_TEXT   tr("В7-80/2 функция 0x41: Сброс в исходное состояние.")

#define V780_CMD_0x42     0x42
#define V780_CMD_0x42_TEXT   tr("В7-80/2 функция 0x42: Установка скорости обмена.")

#define V780_CMD_0x45     0x45
#define V780_CMD_0x45_TEXT   tr("В7-80/2 функция 0x45: Инициализация вторичной станции.")

#define V780_CMD_0x46     0x46
#define V780_CMD_0x46_TEXT   tr("В7-80/2 функция 0x46: Запрос идентификатора.")

#define V780_CMD_0x47     0x47
#define V780_CMD_0x47_TEXT   tr("В7-80/2 функция 0x47: Запрос состояния.")

#define V780_CMD_0x48     0x48
#define V780_CMD_0x48_TEXT   tr("В7-80/2 функция 0x48: Переадресация.")

#define V780_CMD_0x53     0x53
#define V780_CMD_0x53_TEXT   tr("В7-80/2 функция 0x53: Управление прибором.")
//--------------------------------------------------------------------------------
typedef struct
{
    unsigned char  address;
    unsigned char  cmd;
    unsigned char  count_data;
} V780_HEADER;
//--------------------------------------------------------------------------------
union CMD_V780_ERROR_ANSWER
{
    struct BODY{
        V780_HEADER header;
        unsigned char error;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union CMD_V780_0x41_QUESTION
{
    struct BODY{
        V780_HEADER header;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union CMD_V780_0x42_QUESTION
{
    struct BODY{
        V780_HEADER header;
        struct {
            unsigned char  baud;
            unsigned char  time;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union CMD_V780_0x45_QUESTION
{
    struct BODY{
        V780_HEADER header;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union CMD_V780_0x45_ANSWER
{
    struct BODY{
        V780_HEADER header;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union CMD_V780_0x46_QUESTION
{
    struct BODY{
        V780_HEADER header;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union CMD_V780_0x46_ANSWER
{
    struct BODY{
        V780_HEADER header;
        struct {
            unsigned char  type;
            unsigned char  year;
            unsigned char  month;
            unsigned char  serno_lo;
            unsigned char  serno_hi;
            unsigned char  baud;
            unsigned char  time_waiting;
            unsigned char  address_station;
            unsigned char  byte_test;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union CMD_V780_0x47_QUESTION
{
    struct BODY{
        V780_HEADER header;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union CMD_V780_0x47_ANSWER
{
    struct BODY{
        V780_HEADER header;
        struct {
            unsigned char  state;
            unsigned char  error;
            signed int     value;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union CMD_V780_0x48_QUESTION
{
    struct BODY{
        V780_HEADER header;
        struct {
            unsigned char  data_0;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union CMD_V780_0x48_ANSWER
{
    struct BODY{
        V780_HEADER header;
        struct {
            unsigned char  address_station;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------------------------------------------
union CMD_V780_0x53_QUESTION
{
    struct BODY{
        V780_HEADER header;
        struct {
            unsigned char  data_0;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union CMD_V780_0x53_ANSWER
{
    struct BODY{
        V780_HEADER header;
        struct {
            unsigned char  data_0;
        } data;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};

union CMD_V780_0x53_ANSWER_ERROR
{
    struct BODY{
        V780_HEADER header;
        unsigned char error;
        unsigned short crc16;
    } body;
    unsigned char buf[sizeof(BODY)];
};
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif
