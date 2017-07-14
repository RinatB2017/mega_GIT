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
#include "demo_device_base.hpp"
#include "demo_device_td.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_td.h"
//--------------------------------------------------------------------------------
#include "pet_errno.h"
//--------------------------------------------------------------------------------
Device_TD::Device_TD(QObject *parent) : Device_base(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Device_TD::init(void)
{
    connect_log();

    base_func_list.append(&Device_base::f_PET_CMD_TEST);
    base_func_list.append(&Device_base::f_PET_CMD_RESET);
    base_func_list.append(&Device_base::f_PET_CMD_SHUTDOWN);
    base_func_list.append(&Device_base::f_PET_CMD_DISCOVERY);
    base_func_list.append(&Device_base::f_PET_CMD_CONFIG_SAVE);
    base_func_list.append(&Device_base::f_PET_CMD_CONFIG_DEFAULT);
    base_func_list.append(&Device_base::f_PET_CMD_GET_DEVICE_INFO);
    base_func_list.append(&Device_base::f_PET_CMD_WAVE);

    base_func_list.append(&Device_base::f_PET_CMD_RUN);
    base_func_list.append(&Device_base::f_PET_CMD_STOP);

    td_func_list.append(&Device_TD::f_PET_CMD_SET_CONTROL);
    td_func_list.append(&Device_TD::f_PET_CMD_GET_CONTROL);
    td_func_list.append(&Device_TD::f_PET_CMD_GET_STATE);
    td_func_list.append(&Device_TD::f_PET_CMD_GET_CH_STATE);
    td_func_list.append(&Device_TD::f_PET_CMD_GET_CH_CONTROL);
    td_func_list.append(&Device_TD::f_PET_CMD_SET_CH_CONTROL);
    td_func_list.append(&Device_TD::f_PET_CMD_UPDATE);

    //---
    state.state = 0;
    //---
    ch_state.channel = 0;
    ch_state.state = 0;
    //---
    control.trn_ctrl = 0; //FIXME
    control.timeout = 0;
    //---
    ch_ctrl.channel = 0;
    ch_ctrl.ctrl = 0;
    //---
    state.state         = 0;    /* Состояние БОС (одно из значений PET_BOS_STATE) */
    state.flags         = 1;    /* Флаги состояния */
    state.sync_cycle    = 2;    /* Номер текущего цикла синхронизации */
    state.utilisation   = 3;    /* Общаяя загрузка по всем каналам регистрации */

    ch_state.channel        = 0;    /* Номер канала */
    ch_state.state          = 1;    /* Состояние канала (одно из значений PET_BOS_CH_STATE)*/
    ch_state.flags          = 2;    /* Флаги состояния */
    ch_state.sync_cycle     = 3;    /* Номер цикла синхронизации */
    ch_state.utilisation    = 4;    /* Загрузка канала регистрации */
}
//--------------------------------------------------------------------------------
void Device_TD::connect_log(void)
{
#ifdef NO_LOG
    connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
#else
    if(parent())
    {
        connect(this, SIGNAL(info(QString)),    parent(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent(), SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), parent(), SIGNAL(message(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
    }
#endif
}
//--------------------------------------------------------------------------------
PET_RESULT Device_TD::check(QByteArray input_packet, QByteArray *output_packet)
{
    foreach (base_func f0, base_func_list)
    {
        PET_RESULT res = PET_ERR_NONE;
        res = (this->*f0)(input_packet, output_packet);
        if(res == PET_SUCCESS)
        {
            //emit debug(input_packet.toHex().toUpper());
            return PET_SUCCESS;
        }
    }
    foreach (td_func f0, td_func_list)
    {
        PET_RESULT res = PET_ERR_NONE;
        res = (this->*f0)(input_packet, output_packet);
        if(res == PET_SUCCESS)
        {
            //emit debug(input_packet.toHex().toUpper());
            return PET_SUCCESS;
        }
    }
    return PET_ERR_FAULT;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_TD::f_PET_CMD_SET_CONTROL(QByteArray input_packet,
                                            QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_SET_CONTROL);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_SET_CONTROL");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != sizeof(pet_td_ctrl_t))
    {
        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    //TODO
    memcpy((char *)&control, (char *)&i_frame->data[0], sizeof(control));

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(control);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));
    (*output_packet).append((char *)&control, sizeof(control));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_TD::f_PET_CMD_GET_CONTROL(QByteArray input_packet,
                                            QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_GET_CONTROL);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_GET_CONTROL");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != 0)
    {
        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(control);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,   sizeof(o_frame));
    (*output_packet).append((char *)&control,   sizeof(control));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_TD::f_PET_CMD_GET_STATE(QByteArray input_packet,
                                          QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_GET_STATE);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_GET_STATE");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != 0)
    {
        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(state);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,   sizeof(o_frame));
    (*output_packet).append((char *)&state,     sizeof(state));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_TD::f_PET_CMD_GET_CH_STATE(QByteArray input_packet,
                                             QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_GET_CH_STATE);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_GET_CH_STATE");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != 2)
    {
        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(ch_state);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,   sizeof(o_frame));
    (*output_packet).append((char *)&ch_state,  sizeof(ch_state));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_TD::f_PET_CMD_GET_CH_CONTROL(QByteArray input_packet,
                                               QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_GET_CH_CONTROL);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_GET_CH_CONTROL");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != 2)
    {
        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(ch_ctrl);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,   sizeof(o_frame));
    (*output_packet).append((char *)&ch_ctrl,   sizeof(ch_ctrl));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_TD::f_PET_CMD_SET_CH_CONTROL(QByteArray input_packet,
                                               QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_SET_CH_CONTROL);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_SET_CH_CONTROL");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != sizeof(ch_ctrl))
    {
        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    //TODO
    memcpy((char *)&ch_ctrl, (char *)&i_frame->data[0], sizeof(ch_ctrl));

    (*output_packet).clear();
    (*output_packet).append(input_packet);
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_TD::f_PET_CMD_UPDATE(QByteArray input_packet,
                                       QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_UPDATE);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_UPDATE");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != (int)sizeof(pet_update_data))
    {
        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = 0;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void Device_TD::set_net_address(QHostAddress value)
{
    if(value.isNull())
    {
        return;
    }

    UINT32_BYTES temp;
    temp.value = value.toIPv4Address();

    control.ip_address[0] = temp.bytes.d;
    control.ip_address[1] = temp.bytes.c;
    control.ip_address[2] = temp.bytes.b;
    control.ip_address[3] = temp.bytes.a;
}
//--------------------------------------------------------------------------------
QHostAddress Device_TD::get_net_address(void)
{
    UINT32_BYTES temp;
    temp.bytes.d = control.ip_address[0];
    temp.bytes.c = control.ip_address[1];
    temp.bytes.b = control.ip_address[2];
    temp.bytes.a = control.ip_address[3];

    return QHostAddress(temp.value);
}
//--------------------------------------------------------------------------------
void Device_TD::set_event_address(QHostAddress value)
{
    if(value.isNull())
    {
        return;
    }

    UINT32_BYTES temp;
    temp.value = value.toIPv4Address();

    control.event_addr[3] = temp.bytes.a;
    control.event_addr[2] = temp.bytes.b;
    control.event_addr[1] = temp.bytes.c;
    control.event_addr[0] = temp.bytes.d;
}
//--------------------------------------------------------------------------------
QHostAddress Device_TD::get_event_address(void)
{
    UINT32_BYTES temp;
    temp.bytes.d = control.event_addr[0];
    temp.bytes.c = control.event_addr[1];
    temp.bytes.b = control.event_addr[2];
    temp.bytes.a = control.event_addr[3];

    return QHostAddress(temp.value);
}
//--------------------------------------------------------------------------------
void Device_TD::set_net_mask(quint32 value)
{
    UINT32_BYTES temp;
    temp.value = value;

    control.netmask[0] = temp.bytes.d;
    control.netmask[1] = temp.bytes.c;
    control.netmask[2] = temp.bytes.b;
    control.netmask[3] = temp.bytes.a;
}
//--------------------------------------------------------------------------------
void Device_TD::set_net_gateway(quint32 value)
{
    UINT32_BYTES temp;
    temp.value = value;

    control.gateway[0] = temp.bytes.d;
    control.gateway[1] = temp.bytes.c;
    control.gateway[2] = temp.bytes.b;
    control.gateway[3] = temp.bytes.a;
}
//--------------------------------------------------------------------------------
void Device_TD::set_mac_address(uint8_t mac_0,
                                uint8_t mac_1,
                                uint8_t mac_2,
                                uint8_t mac_3,
                                uint8_t mac_4,
                                uint8_t mac_5)
{
    control.mac_addr[5] = mac_0;
    control.mac_addr[4] = mac_1;
    control.mac_addr[3] = mac_2;
    control.mac_addr[2] = mac_3;
    control.mac_addr[1] = mac_4;
    control.mac_addr[0] = mac_5;
}
//--------------------------------------------------------------------------------
QString Device_TD::get_str_mac_address(void)
{
    return QString("%1:%2:%3:%4:%5:%6")
            .arg(control.mac_addr[5], 2, 16, QChar('0'))
            .arg(control.mac_addr[4], 2, 16, QChar('0'))
            .arg(control.mac_addr[3], 2, 16, QChar('0'))
            .arg(control.mac_addr[2], 2, 16, QChar('0'))
            .arg(control.mac_addr[1], 2, 16, QChar('0'))
            .arg(control.mac_addr[0], 2, 16, QChar('0'))
            .toUpper();
}
//--------------------------------------------------------------------------------
void Device_TD::set_cmd_port(quint16 value)
{
    cmd_port = value;
}
//--------------------------------------------------------------------------------
quint16 Device_TD::get_cmd_port(void)
{
    return cmd_port;
}
//--------------------------------------------------------------------------------
void Device_TD::set_event_port(quint16 value)
{
    event_port = value;
}
//--------------------------------------------------------------------------------
void Device_TD::set_pet_address(uint8_t value)
{
    pet_address = value;
}
//--------------------------------------------------------------------------------
uint8_t Device_TD::get_pet_address(void)
{
    return pet_address;
}
//--------------------------------------------------------------------------------
