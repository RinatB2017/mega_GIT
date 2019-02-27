/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
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
#include <QTest>
//--------------------------------------------------------------------------------
#include "test.hpp"
//--------------------------------------------------------------------------------
#include "crc.h"
//--------------------------------------------------------------------------------
Test::Test()
{

}
//--------------------------------------------------------------------------------
Test::~Test()
{

}
//--------------------------------------------------------------------------------
void Test::test_protocol(void)
{
    QByteArray question;
    QByteArray answer;

    Test_protocol *proto = new Test_protocol;
    //proto->add_command(&cmd_1);

    HEADER header;
    uint16_t crc16 = 0;
    int result = 0;

    DATA_CMD_1 data_cmd_1;
    DATA_CMD_2 data_cmd_2;

    //---
    header.addr = 0;
    header.cmd = CMD_1;
    header.len = sizeof(DATA_CMD_1);

    question.clear();
    question.append((char *)&header,     sizeof(HEADER));
    question.append((char *)&data_cmd_1, sizeof(DATA_CMD_1));

    crc16 = CRC::crc16((uint8_t *)question.data(), question.length());
    question.append((char *)&crc16,  sizeof(crc16));

    result = proto->check_packet(question, &answer);
    QCOMPARE(result, (int)Base_protocol::E_NO_ERROR);
    //---
    header.addr = 0;
    header.cmd = CMD_2;
    header.len = sizeof(DATA_CMD_2);

    question.clear();
    question.append((char *)&header,     sizeof(HEADER));
    question.append((char *)&data_cmd_2, sizeof(DATA_CMD_2));

    crc16 = CRC::crc16((uint8_t *)question.data(), question.length());
    question.append((char *)&crc16,  sizeof(crc16));

    result = proto->check_packet(question, &answer);
    QCOMPARE(result, (int)Base_protocol::E_NO_ERROR);
    //---
}

//--------------------------------------------------------------------------------
