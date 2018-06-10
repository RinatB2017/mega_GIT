/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#ifndef TEST_PROTOCOL_HPP
#define TEST_PROTOCOL_HPP
//--------------------------------------------------------------------------------
#include "base_protocol.hpp"
//--------------------------------------------------------------------------------
#pragma pack (push, 1)
//--------------------------------------------------------------------------------
typedef struct DATA_CMD_1
{
    uint8_t  a;
    uint8_t  b;
    uint8_t  c;
} data_cmd_1_t;

typedef struct DATA_CMD_2
{
    uint8_t  a;
    uint8_t  b;
    uint8_t  c;
    uint8_t  d;
    uint8_t  e;
    uint8_t  f;
} data_cmd_2_t;
//--------------------------------------------------------------------------------
#pragma pack(pop)
//-------------------------------------------------------------------------------
enum {
    CMD_1 = 1,
    CMD_2 = 2,
    CMD_3 = 3
};
//-------------------------------------------------------------------------------
class Test_protocol : public Base_protocol
{
    Q_OBJECT

public:
    Test_protocol();
    void set_address(uint8_t new_address);

private:
    int command(uint8_t cmd, QByteArray data);

};
//--------------------------------------------------------------------------------
#endif // TEST_PROTOCOL_HPP
