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
#include "demo_device_brg.hpp"
#include "demo_device_base.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
//--------------------------------------------------------------------------------
Device_BRG::Device_BRG(QObject *parent) : Device_base(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Device_BRG::init(void)
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

    brg_func_list.append(&Device_BRG::f_PET_CMD_SET_CALIBRATION);
    brg_func_list.append(&Device_BRG::f_PET_CMD_GET_CALIBRATION);

    brg_func_list.append(&Device_BRG::f_PET_CMD_SET_CONTROL);
    brg_func_list.append(&Device_BRG::f_PET_CMD_GET_CONTROL);
    brg_func_list.append(&Device_BRG::f_PET_CMD_SET_CH_CONTROL);
    brg_func_list.append(&Device_BRG::f_PET_CMD_GET_CH_CONTROL);
    brg_func_list.append(&Device_BRG::f_PET_CMD_GET_STATE);
    brg_func_list.append(&Device_BRG::f_PET_CMD_GET_CH_STATE);
    brg_func_list.append(&Device_BRG::f_PET_CMD_UPDATE);

    //---
    memset((char *)&calibration, 0, sizeof(calibration));
    memset((char *)&control, 0, sizeof(control));
    memset((char *)&state, 0, sizeof(state));
    memset((char *)&ch_state, 0, sizeof(ch_state));
    memset((char *)&ch_ctrl, 0, sizeof(ch_ctrl));

    ch_state.state = PET_BRG_STATE_SCAN;
    ch_state.flags = PET_BRG_STATE_FLAG_SYNC_CYCLE_MSK | PET_BRG_STATE_FLAG_UTILISATION_MSK;
    ch_state.utilisation = rand() % 0xFFFF;
    ch_state.sync_cycle = 666;

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

    for(int n=0; n<PET_BRG_CH_NUM; n++)
    {
        calibration.ch_calibration[n].channel                   = n;    /* Номер канала */
        calibration.ch_calibration[n].flags                     = 0;    /* Флаги */
        calibration.ch_calibration[n].timestamp                 = 1;    /* Календарное время выполнения последней калибровки*/
        calibration.ch_calibration[n].xp_offset                 = 2;    /* Смещение сигнала X+ (мВ) */
        calibration.ch_calibration[n].xp_gain                   = 3;    /* Наклон сигнала X+ (1/мВ) */
        calibration.ch_calibration[n].xn_offset                 = 4;    /* Смещение сигнала X- (мВ) */
        calibration.ch_calibration[n].xn_gain                   = 5;    /* Наклон сигнала X- (1/мВ) */
        calibration.ch_calibration[n].yp_offset                 = 6;    /* Смещение сигнала Y+ (мВ) */
        calibration.ch_calibration[n].yp_gain                   = 7;    /* Наклон сигнала Y+ (1/мВ) */
        calibration.ch_calibration[n].yn_offset                 = 8;    /* Смещение сигнала Y- (мВ) */
        calibration.ch_calibration[n].yn_gain                   = 9;    /* Наклон сигнала Y- (1/мВ) */
        calibration.ch_calibration[n].ts_offset                 = 10;   /* Поправка метки времени (пс) */
        calibration.ch_calibration[n].in_offset_set_offset      = 11;   /* Смещение сигнала ФЭУ, смещение установки (мВ) */
        calibration.ch_calibration[n].in_offset_set_gain        = 12;   /* Смещение сигнала ФЭУ, наклон установки (1/мВ) */
        calibration.ch_calibration[n].in_offset_get_offset      = 13;   /* Смещение сигнала ФЭУ, смещение измерения (мВ) */
        calibration.ch_calibration[n].in_offset_get_gain        = 14;   /* Смещение сигнала ФЭУ, наклон измерения (1/мВ) */
        calibration.ch_calibration[n].in_threshold_set_offset   = 15;   /* Порог дискриминации, смещение установки (мВ) */
        calibration.ch_calibration[n].in_threshold_set_gain     = 16;   /* Порог дискриминации, наклон установки (1/мВ) */
        calibration.ch_calibration[n].in_threshold_get_offset   = 17;   /* Порог дискриминации, смещение измерения (мВ) */
        calibration.ch_calibration[n].in_thresholdga_get_gain   = 18;   /* Порог дискриминации, наклон измерения (1/мВ) */
        calibration.ch_calibration[n].adc_ref_set_offset        = 19;   /* Опорное напряжение АЦП, смещение установки (мВ) */
        calibration.ch_calibration[n].adc_ref_set_gain          = 20;   /* Опорное напряжение АЦП, наклон установки (1/мВ) */
        calibration.ch_calibration[n].adc_ref_get_offset        = 21;   /* Опорное напряжение АЦП, смещение измерения (мВ) */
        calibration.ch_calibration[n].adc_ref_get_gain          = 22;   /* Опорное напряжение АЦП, наклон измерения (1/мВ) */
        calibration.ch_calibration[n].pm_bias_set_offset        = 23;   /* Напряжение смещения ФЭУ, смещение установки (мВ) */
        calibration.ch_calibration[n].pm_bias_set_gain          = 24;   /* Напряжение смещения ФЭУ, наклон установки (1/мВ) */
        calibration.ch_calibration[n].pm_bias_get_offset        = 25;   /* Напряжение смещения ФЭУ, смещение измерения (мВ) */
        calibration.ch_calibration[n].pm_bias_get_gain          = 26;   /* Напряжение смещения ФЭУ, наклон измерения (1/мВ) */
        calibration.ch_calibration[n].readout_temp_get_offset   = 27;   /* Температура платы считывания, смещение измерения (х0,01 °C) */
        calibration.ch_calibration[n].readout_temp_get_gain     = 28;   /* Температура платы считывания, наклон измерения (х0,01 1/°C) */
        calibration.ch_calibration[n].ifmodule_temp_get_offset  = 29;   /* Температура модуля сопряжения, смещение измерения (х0,01 °C) */
        calibration.ch_calibration[n].ifmodule_temp_get_gain    = 30;   /* Температура модуля сопряжения, наклон измерения (х0,01 1/°C) */
    }
}
//--------------------------------------------------------------------------------
void Device_BRG::connect_log(void)
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
PET_RESULT Device_BRG::check(QByteArray input_packet, QByteArray *output_packet)
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
    foreach (brg_func f0, brg_func_list)
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
PET_RESULT Device_BRG::f_PET_CMD_SET_CALIBRATION(QByteArray input_packet,
                                                 QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_SET_CALIBRATION);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_SET_CALIBRATION");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != sizeof(pet_brg_calibration))
    {
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error(QString("sizeof(pet_brg_calibration) = %1").arg(sizeof(pet_brg_calibration)));
        emit error("i_frame->len != sizeof(pet_brg_calibration)");

        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    //---
    try
    {
        memcpy((char *)&calibration, (char *)i_frame->data, sizeof(calibration));
    }
    catch(...)
    {
        emit error("f_PET_CMD_SET_CALIBRATION");
    }
    //---

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(calibration);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));
    (*output_packet).append((char *)&calibration, sizeof(calibration));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_BRG::f_PET_CMD_GET_CALIBRATION(QByteArray input_packet,
                                                 QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_GET_CALIBRATION);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_GET_CALIBRATION");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    if(i_frame->len != 2)
    {
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error("i_frame->len != 2");

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
    o_frame.len = sizeof(calibration);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,       sizeof(o_frame));
    (*output_packet).append((char *)&calibration,   sizeof(calibration));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_BRG::f_PET_CMD_SET_CONTROL(QByteArray input_packet,
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
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error(QString("sizeof(pet_brg_ctrl) = %1").arg(sizeof(pet_brg_ctrl)));
        emit error("i_frame->len != sizeof(pet_brg_ctrl)");

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
PET_RESULT Device_BRG::f_PET_CMD_GET_CONTROL(QByteArray input_packet,
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
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error("i_frame->len != 0");

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
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));
    (*output_packet).append((char *)&control, sizeof(control));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_BRG::f_PET_CMD_GET_STATE(QByteArray input_packet,
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
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error("i_frame->len != 0");

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
PET_RESULT Device_BRG::f_PET_CMD_GET_CH_STATE(QByteArray input_packet,
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
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error("i_frame->len != 2");

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
PET_RESULT Device_BRG::f_PET_CMD_SET_CH_CONTROL(QByteArray input_packet,
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
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error(QString("sizeof(ch_ctrl) = %1").arg(sizeof(ch_ctrl)));
        emit error("i_frame->len != sizeof(ch_ctrl)");

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
    o_frame.len = sizeof(ch_ctrl);

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,   sizeof(o_frame));
    (*output_packet).append((char *)&ch_ctrl,   sizeof(ch_ctrl));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_BRG::f_PET_CMD_GET_CH_CONTROL(QByteArray input_packet,
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
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error("i_frame->len != 2");

        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;
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
PET_RESULT Device_BRG::f_PET_CMD_UPDATE(QByteArray input_packet,
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
    emit debug(QString("size input_packet = %1").arg(input_packet.length()));
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();
    pet_update_data_t *i_data = (pet_update_data_t *)i_frame->data;

    if(i_frame->len != ((int)sizeof(pet_update_data_t) + i_data->length))
    {
        emit error(QString("i_frame->len = %1").arg(i_frame->len));
        emit error(QString("sizeof(pet_update_data) = %1").arg((sizeof(pet_update_data_t))));
        emit error(QString("i_data->length = %1").arg((i_data->length)));
        emit error("i_frame->len != ((int)sizeof(pet_update_data) + i_data->length)");

        pet_frame_error o_frame;
        o_frame.addr  = pet_address;
        o_frame.cmd   = i_frame->cmd | PET_FRAME_CMD_FAULT_MSK;
        o_frame.len   = sizeof(o_frame.result_cmd);
        o_frame.result_cmd  = PET_ERR_BAD_FRAME;

        (*output_packet).clear();
        (*output_packet).append((char *)&o_frame, sizeof(o_frame));
        return PET_SUCCESS;
    }

    i_frame->len = 0;

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = 0;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void Device_BRG::set_pet_address(uint8_t value)
{
    pet_address = value;
}
//--------------------------------------------------------------------------------
uint8_t Device_BRG::get_pet_address(void)
{
    return pet_address;
}
//--------------------------------------------------------------------------------
