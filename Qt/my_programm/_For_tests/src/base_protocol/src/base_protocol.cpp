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
#include "base_protocol.hpp"
#include "crc.h"
//--------------------------------------------------------------------------------
Base_protocol::Base_protocol()
{

}
//--------------------------------------------------------------------------------
int Base_protocol::check_packet(QByteArray question,
                                QByteArray *answer)
{
    if(question.isEmpty())
    {
        return E_PACKET_EMPTY;
    }
    if(question.length() <= (int)sizeof(HEADER))
    {
        return E_BAD_SIZE;
    }
    HEADER *header = (HEADER *)question.data();
    if(address != header->addr)
    {
        return E_BAD_ADDRESS;
    }

    int calc_len = question.length() - sizeof(HEADER) - 2;
    if(calc_len != header->len)
    {
        return E_BAD_SIZE;
    }
    uint16_t calc_crc16 = CRC::crc16((uint8_t *)question.data(), question.length() - 2);
    uint8_t a = question[question.length() - 1];
    uint8_t b = question[question.length() - 2];
    uint16_t packet_crc16 = (a << 8) | b;
    if(packet_crc16 != calc_crc16)
    {
        return E_BAD_CRC16;
    }

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void Base_protocol::add_command(CMD cmd)
{
    commands.append(cmd);
}
//--------------------------------------------------------------------------------
