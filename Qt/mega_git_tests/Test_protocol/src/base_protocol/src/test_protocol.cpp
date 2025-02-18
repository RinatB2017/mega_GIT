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
#include "test_protocol.hpp"
//--------------------------------------------------------------------------------
Test_protocol::Test_protocol()
{

}
//--------------------------------------------------------------------------------
void Test_protocol::set_address(uint8_t new_address)
{
    address = new_address;
}
//--------------------------------------------------------------------------------
int Test_protocol::command(uint8_t cmd,
                           QByteArray data)
{
    switch(cmd)
    {
    case CMD_1:
        if(data.length() != sizeof(DATA_CMD_1))
        {
            emit error(QString("command: %1 data len %2")
                      .arg(cmd)
                      .arg(data.size()));
            return E_BAD_DATA;
        }
        emit info("CMD_1: OK");
        break;

    case CMD_2:
        if(data.length() != sizeof(DATA_CMD_2))
        {
            emit error(QString("command: %1 data len %2")
                      .arg(cmd)
                      .arg(data.size()));
            return E_BAD_DATA;
        }
        emit info("CMD_2: OK");
        break;

    case CMD_3:
        emit info("CMD_3");
        break;

    default:
        return E_BAD_CMD;
    }
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
