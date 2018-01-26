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
#include "b590_packets.hpp"
#include "upacket.hpp"
//--------------------------------------------------------------------------------
UPacket::UPacket(void) :
    QObject()
{

}
//--------------------------------------------------------------------------------
void UPacket::set_address(int address)
{
    addr = address;
    emit info(QString("set address: %1").arg(addr));
}
//--------------------------------------------------------------------------------
int UPacket::get_address(void)
{
    return addr;
}
//--------------------------------------------------------------------------------
bool UPacket::check_packet(QByteArray packet, int *cmd, QByteArray *data)
{
    Q_CHECK_PTR(data);

    B590_HEADER *header = (B590_HEADER *)packet.data();
    Q_CHECK_PTR(header);

    int full_packet_len = sizeof(B590_HEADER) + header->count_data + 2;
    if(packet.size() != full_packet_len)
    {
        err = ERR_BAD_LEN;
        return false;
    }

    if(header->address != addr)
    {
        err = ERR_BAD_ADDR;
        return false;
    }

    *cmd = header->cmd;

    return true;
}
//--------------------------------------------------------------------------------
int UPacket::get_err(void)
{
    return err;
}
//--------------------------------------------------------------------------------
QString UPacket::get_err_str(void)
{
    QString temp;

    switch (err)
    {
    case ERR_BAD_PACKET:
        temp = "ERR_BAD_PACKET";
        break;

    case ERR_BAD_ADDR:
        temp = "ERR_BAD_ADDR";
        break;

    case ERR_BAD_CMD:
        temp = "ERR_BAD_CMD";
        break;

    case ERR_BAD_LEN:
        temp = "ERR_BAD_LEN";
        break;

    case ERR_BAD_CRC:
        temp = "ERR_BAD_CRC";
        break;

    default:
        temp = QString("unknown err code: %1").arg(err);
        break;
    }

    return temp;
}
//--------------------------------------------------------------------------------
