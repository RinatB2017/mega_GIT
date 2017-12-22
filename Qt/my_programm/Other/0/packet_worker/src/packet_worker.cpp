/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#include <QObject>
//--------------------------------------------------------------------------------
#include "packet_worker.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
//--------------------------------------------------------------------------------
Packet_worker::Packet_worker(QObject *parent) : QObject(parent)
{
    input_buffer.clear();
    output_buffer.clear();
}
//--------------------------------------------------------------------------------
void Packet_worker::get_data(QByteArray data)
{
    input_buffer.clear();
    input_buffer.append(data);

    check_main_header();
}
//--------------------------------------------------------------------------------
int Packet_worker::get_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
QString Packet_worker::get_error_text(void)
{
    return last_error_text;
}
//--------------------------------------------------------------------------------
bool Packet_worker::set_address(int address)
{
    this->address = address;
    return true;
}
//--------------------------------------------------------------------------------
bool Packet_worker::set_param_channel(int channel)
{
    this->param_channel = channel;
    return true;
}
//--------------------------------------------------------------------------------
bool Packet_worker::set_param_id(int id)
{
    this->param_id = id;
    return true;
}
//--------------------------------------------------------------------------------
bool Packet_worker::set_param_value(int value)
{
    this->param_value = value;
    return true;
}
//--------------------------------------------------------------------------------
bool Packet_worker::check_main_header(void)
{
    //FIXME
    return false;
}
//--------------------------------------------------------------------------------
QByteArray Packet_worker::get_input_buffer(void)
{
    return input_buffer;
}
//--------------------------------------------------------------------------------
QByteArray Packet_worker::get_output_buffer(void)
{
    return output_buffer;
}
//--------------------------------------------------------------------------------
