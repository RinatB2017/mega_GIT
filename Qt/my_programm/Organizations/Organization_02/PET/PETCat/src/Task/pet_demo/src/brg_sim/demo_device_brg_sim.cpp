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
#include "demo_device_brg_sim.hpp"
#include "demo_device_base.hpp"
//--------------------------------------------------------------------------------
#include "pet_brg_sim.h"
//--------------------------------------------------------------------------------
Device_BRG_SIM::Device_BRG_SIM(QObject *parent) : Device_base(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Device_BRG_SIM::init(void)
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

    brg_sim_func_list.append(&Device_BRG_SIM::f_PET_CMD_SET_CONTROL);
    brg_sim_func_list.append(&Device_BRG_SIM::f_PET_CMD_GET_CONTROL);
    brg_sim_func_list.append(&Device_BRG_SIM::f_PET_CMD_SET_CH_CONTROL);
    brg_sim_func_list.append(&Device_BRG_SIM::f_PET_CMD_GET_CH_CONTROL);
    brg_sim_func_list.append(&Device_BRG_SIM::f_PET_CMD_GET_STATE);
    brg_sim_func_list.append(&Device_BRG_SIM::f_PET_CMD_GET_CH_STATE);
    brg_sim_func_list.append(&Device_BRG_SIM::f_PET_CMD_UPDATE);

    //---
    memset((char *)&control, 0, sizeof(control));
    memset((char *)&state, 0, sizeof(state));
    memset((char *)&ch_state, 0, sizeof(ch_state));
    memset((char *)&ch_ctrl, 0, sizeof(ch_ctrl));
    //---
    state.state             = 0;    /* Состояние БРГ (одно из значений PET_BRG_STATE) */
    state.flags             = 1;    /* Флаги состояния */
    state.sync_cycle        = 2;    /* Номер текущего цикла синхронизации */
    state.utilisation       = 3;    /* Общаяя загрузка по всем каналам регистрации */

    ch_state.channel        = 0;    /* Номер канала */
    ch_state.state          = 1;    /* Состояние канала (одно из значений PET_BRG_CH_STATE)*/
    ch_state.flags          = 2;    /* Флаги состояния канала обработки */
    ch_state.in_offset      = 3;    /* Смещение сигнала ФЭУ (мВ) */
    ch_state.in_threshold   = 4;    /* Порог дискриминации (мВ) */
    ch_state.adc_reference  = 5;    /* Опорное напряжение АЦП (мВ) */
    ch_state.pm_bias        = 6;    /* Напряжение смещения ФЭУ (мВ) */
    ch_state.readout_temp   = 7;    /* Температура платы считывания (х0,01 °C) */
    ch_state.ifmodule_temp  = 8;    /* Температура модуля сопряжения (х0,01 °C) */
    ch_state.sync_cycle     = 9;    /* Номер цикла синхронизации */
    ch_state.utilisation    = 10;   /* Загрузка канала регистрации */
    ch_state.adc_underflow  = 11;   /* Число событий с признаком опустошения (потери значимости) данных АЦП */
    ch_state.adc_overflow   = 12;   /* Число событий с признаком переполнения (потери значимости) данных АЦП */
    ch_state.qdc_underflow  = 13;   /* Число событий с признаком опустошения (потери значимости) координатных сигналов */
    ch_state.qdc_overflow   = 14;   /* Число событий с признаком переполнения (потери значимости) координатных сигналов */
    ch_state.gate_overlay   = 15;   /* Число событий с признаком наложения событий */
    ch_state.ts_invalid     = 16;   /* Число событий с признаком не корректности метки времени */
}
//--------------------------------------------------------------------------------
void Device_BRG_SIM::connect_log(void)
{
#ifdef NO_LOG
    connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));
#else
    if(parent())
    {
        connect(this, SIGNAL(info(QString)),    parent(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent(), SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   parent(), SIGNAL(trace(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(trace(QString)),   this, SLOT(log(QString)));
    }
#endif
}
//--------------------------------------------------------------------------------
PET_RESULT Device_BRG_SIM::check(QByteArray input_packet, QByteArray *output_packet)
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
    foreach (brg_sim_func f0, brg_sim_func_list)
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
PET_RESULT Device_BRG_SIM::f_PET_CMD_SET_CONTROL(QByteArray input_packet,
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

    if(i_frame->len != sizeof(pet_brg_ctrl))
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
PET_RESULT Device_BRG_SIM::f_PET_CMD_GET_CONTROL(QByteArray input_packet,
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
PET_RESULT Device_BRG_SIM::f_PET_CMD_GET_STATE(QByteArray input_packet,
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
PET_RESULT Device_BRG_SIM::f_PET_CMD_GET_CH_STATE(QByteArray input_packet,
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
PET_RESULT Device_BRG_SIM::f_PET_CMD_SET_CH_CONTROL(QByteArray input_packet,
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

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(control);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,   sizeof(o_frame));
    (*output_packet).append((char *)&ch_ctrl,   sizeof(ch_ctrl));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_BRG_SIM::f_PET_CMD_GET_CH_CONTROL(QByteArray input_packet,
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
PET_RESULT Device_BRG_SIM::f_PET_CMD_UPDATE(QByteArray input_packet,
                                            QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr) return PET_ERR_FAULT;
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_UPDATE);
    if(res != PET_SUCCESS)  return res;

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
    (*output_packet).append((char *)&o_frame,   sizeof(o_frame));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void Device_BRG_SIM::set_pet_address(uint8_t value)
{
    pet_address = value;
}
//--------------------------------------------------------------------------------
uint8_t Device_BRG_SIM::get_pet_address(void)
{
    return pet_address;
}
//--------------------------------------------------------------------------------
