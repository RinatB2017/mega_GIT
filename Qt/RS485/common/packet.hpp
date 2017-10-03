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
#ifndef PACKET_HPP
#define PACKET_HPP
//--------------------------------------------------------------------------------
#pragma pack (push, 1)

enum CMD {
    CMD_TEST    = 0,
    CMD_READ    = 1,
    CMD_WRITE   = 2,
    CMD_RESET   = 3
};

struct HEADER
{
    uint16_t    prefix_16;                  // префикс
    uint8_t     addr_8;                     // адрес модуля
    uint8_t     cmd_8;                      // команда
    uint16_t    len_16;                     // длина данных
};

//--------------------------------------------
union QUESTION_TEST
{
    struct BODY
    {
        HEADER      header;
        uint16_t    data;
        uint16_t    crc16;                  // контрольная сумма
    } body;
    unsigned char buf[sizeof(BODY)];
};

union ANSWER_TEST
{
    struct BODY
    {
        HEADER      header;
        uint16_t    data;
        uint16_t    crc16;                  // контрольная сумма
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------
union QUESTION_RESET
{
    struct BODY
    {
        HEADER      header;
        uint16_t    data;
        uint16_t    crc16;                  // контрольная сумма
    } body;
    unsigned char buf[sizeof(BODY)];
};

union ANSWER_RESET
{
    struct BODY
    {
        HEADER      header;
        uint16_t    data;
        uint16_t    crc16;                  // контрольная сумма
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------
union QUESTION_RW
{
    struct BODY
    {
        HEADER      header;

        uint32_t    addr_cam_32;                // адрес камеры
        uint16_t    time_interval_16;           // интервал дворника
        uint32_t    time_washout_32;            // время помывки
        uint32_t    time_pause_washout_32;      // время между помывками
        uint32_t    preset_washout_32;          // пресет помывки
        uint32_t    time_preset_washout_32;     // времен помывки

        uint16_t    crc16;                      // контрольная сумма
    } body;
    unsigned char buf[sizeof(BODY)];
};

union ANSWER_RW
{
    struct BODY
    {
        HEADER      header;

        uint32_t    addr_cam_32;                // адрес камеры
        uint16_t    time_interval_16;           // интервал дворника
        uint32_t    time_washout_32;            // время помывки
        uint32_t    time_pause_washout_32;      // время между помывками
        uint32_t    preset_washout_32;          // пресет помывки
        uint32_t    time_preset_washout_32;     // времен помывки

        uint16_t    crc16;                      // контрольная сумма
    } body;
    unsigned char buf[sizeof(BODY)];
};
//--------------------------------------------
#pragma pack(pop)
//--------------------------------------------------------------------------------
#endif // PACKET_HPP
