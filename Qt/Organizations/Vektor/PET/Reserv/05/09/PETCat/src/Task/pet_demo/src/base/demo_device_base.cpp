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
#include <QHostAddress>
#include <QtMath>
//--------------------------------------------------------------------------------
#include "demo_device_base.hpp"
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
#include "pet_defines.hpp"
//--------------------------------------------------------------------------------
#include "pet.h"
#include "pet_brg.h"
#include "pet_errno.h"
//--------------------------------------------------------------------------------
Device_base::Device_base(QObject *parent) : QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void Device_base::init(void)
{
    double offset_begin_y = 20000.0;
    double vert_ampl = 10000.0;
    double begin_angle = 90.0;
    double count_cycles = 4.0;
    for(int n=0; n<PET_WAVE_POINT_MAX; n++)
    {
        int16_t value = offset_begin_y + double((double)vert_ampl)*qCos(double(begin_angle + n*5.625*count_cycles)*double(M_PI)/double(180.0));
        test_pattern[n] = value;
    }
}
//--------------------------------------------------------------------------------
void Device_base::log(const QString data)
{
#ifdef NO_LOG
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(mw)
    {
        mw->set_status2_text(data);
    }
    else
    {
        qDebug() << data;
    }
#else
    qDebug() << data;
#endif
}

//--------------------------------------------------------------------------------
PET_RESULT Device_base::check_packet(QByteArray input_packet, uint8_t cmd)
{
    if(input_packet.isEmpty())
    {
        return PET_ERR_FAULT;
    }
    if(input_packet.length() < (int)sizeof(pet_frame_t))
    {
        return PET_ERR_FAULT;
    }

    pet_frame_t *frame = (pet_frame_t *)input_packet.data();
    if(frame->addr != PET_ADDR_BROADCAST)
    {
        if(frame->addr != pet_address)
        {
            return PET_ERR_BAD_ADDR;
        }
    }
    if(frame->cmd != cmd)
    {
        return PET_ERR_BAD_CMD;
    }
    if(input_packet.length() != (int)(frame->len + sizeof(pet_frame_t)))
    {
        emit debug(QString("input_packet.length %1").arg(input_packet.length()));
        emit debug(QString("sizeof(frame) %1").arg(sizeof(pet_frame_t)));
        emit debug(QString("frame->len %1").arg(frame->len));
        return PET_ERR_BAD_FRAME;
    }

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_TEST(QByteArray input_packet,
                                       QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_TEST);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_TEST");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    pet_frame o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd  = i_frame->cmd;
    o_frame.len  = i_frame->len;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,       sizeof(o_frame));
    (*output_packet).append((char *)&i_frame->data, i_frame->len);
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_RESET(QByteArray input_packet,
                                        QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_RESET);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_RESET");
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
    o_frame.cmd  = i_frame->cmd;
    o_frame.len  = 0;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_SHUTDOWN(QByteArray input_packet,
                                           QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_SHUTDOWN);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_SHUTDOWN");
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
    o_frame.cmd  = i_frame->cmd;
    o_frame.len  = 0;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_DISCOVERY(QByteArray input_packet,
                                            QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_DISCOVERY);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_DISCOVERY");
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

    pet_frame_t o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(discovery_info);

    discovery_info.pid = pid;
    discovery_info.vid = vid;
    discovery_info.uid[0] = uid[0];
    discovery_info.uid[1] = uid[1];
    discovery_info.uid[2] = uid[2];
    discovery_info.uid[3] = uid[3];
    discovery_info.uid[4] = uid[4];
    discovery_info.uid[5] = uid[5];

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,           sizeof(o_frame));
    (*output_packet).append((char *)&discovery_info,    sizeof(discovery_info));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_CONFIG_SAVE(QByteArray input_packet,
                                              QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_CONFIG_SAVE);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_CONFIG_SAVE");
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
    o_frame.cmd  = i_frame->cmd;
    o_frame.len  = 0;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));

    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_CONFIG_DEFAULT(QByteArray input_packet,
                                                 QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_CONFIG_DEFAULT);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_CONFIG_DEFAULT");
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

    (*output_packet).clear();
    (*output_packet).append(input_packet);
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_GET_DEVICE_INFO(QByteArray input_packet,
                                                  QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_GET_DEVICE_INFO);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_GET_DEVICE_INFO");
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

    pet_frame_t o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = sizeof(device_info);

    device_info.pid = pid;
    device_info.vid = vid;
    for(unsigned int n=0; n<sizeof(device_info.uid); n++)
    {
        device_info.uid[n] = uid[n];
    }
    device_info.hw_version = 0;
    device_info.sw_version = 666;
    device_info.prot_version = 0;
    device_info.hw_timestamp = 0;
    device_info.sw_timestamp = 0;
    device_info.work_time = 0;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,       sizeof(o_frame));
    (*output_packet).append((char *)&device_info,   sizeof(device_info));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
//FIXME wave
PET_RESULT Device_base::f_PET_CMD_WAVE(QByteArray input_packet,
                                       QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_WAVE);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_WAVE");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();
    pet_wave_t *i_data = (pet_wave_t *)i_frame->data;

    //---
    pet_frame_t o_frame;
    o_frame.addr = pet_address;
    o_frame.cmd = i_frame->cmd;
    o_frame.len = i_frame->len;
    //---
    pet_wave_t o_wave;
    o_wave.channel = i_data->channel;
    o_wave.mode = i_data->mode;
    o_wave.count = i_data->count;
    //---
    float k_x = ((float)rand()) / ((float)RAND_MAX);
    float k_y = ((float)rand()) / ((float)RAND_MAX);
    //---
    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame, sizeof(o_frame));
    (*output_packet).append((char *)&o_wave, sizeof(o_wave));
    for(int i=0; i<i_data->count; i++)
    {
#if 0
        pet_wave_point_t point;
        point.xp = (int16_t)(-k_x * impulse_pattern[i]);
        point.xn = (int16_t)(-(1.0f - k_x) * impulse_pattern[i]);
        point.yp = (int16_t)(-k_y * impulse_pattern[i]);
        point.yn = (int16_t)(-(1.0f - k_y) * impulse_pattern[i]);
        (*output_packet).append((char *)&point,  sizeof(pet_wave_point_t));
#else
        k_x = 0.5;
        k_y = 0.5;

        pet_wave_point_t point;
        point.xp = (int16_t)(k_x * test_pattern[i]);
        point.xn = (int16_t)((1.0f - k_x) * test_pattern[i]);
        point.yp = (int16_t)(k_y * test_pattern[i]);
        point.yn = (int16_t)((1.0f - k_y) * test_pattern[i]);
        (*output_packet).append((char *)&point,  sizeof(pet_wave_point_t));
#endif
    }
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_RUN(QByteArray input_packet, QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_RUN);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_RUN");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    pet_frame o_frame;
    o_frame.addr  = pet_address;
    o_frame.cmd   = i_frame->cmd;
    o_frame.len   = 0;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,       sizeof(o_frame));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT Device_base::f_PET_CMD_STOP(QByteArray input_packet, QByteArray *output_packet)
{
    Q_CHECK_PTR(output_packet);
    if(output_packet == nullptr)
    {
        return PET_ERR_FAULT;
    }
    //---
    PET_RESULT res = PET_SUCCESS;
    res = check_packet(input_packet, PET_CMD_STOP);
    if(res != PET_SUCCESS)
    {
        return res;
    }

    emit debug("f_PET_CMD_STOP");
    pet_frame_t *i_frame = (pet_frame_t *)input_packet.data();

    pet_frame o_frame;
    o_frame.addr  = pet_address;
    o_frame.cmd   = i_frame->cmd;
    o_frame.len   = 0;

    (*output_packet).clear();
    (*output_packet).append((char *)&o_frame,       sizeof(o_frame));
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void Device_base::set_vid(PET_VID value)
{
    vid = value;
}
//--------------------------------------------------------------------------------
PET_VID Device_base::get_vid(void)
{
    return vid;
}
//--------------------------------------------------------------------------------
void Device_base::set_pid(PET_PID value)
{
    pid = value;
}
//--------------------------------------------------------------------------------
PET_PID Device_base::get_pid(void)
{
    return pid;
}
//--------------------------------------------------------------------------------
void Device_base::set_uid(uint8_t uid_0,
                          uint8_t uid_1,
                          uint8_t uid_2,
                          uint8_t uid_3,
                          uint8_t uid_4,
                          uint8_t uid_5)
{
    uid[0] = uid_0;
    uid[1] = uid_1;
    uid[2] = uid_2;
    uid[3] = uid_3;
    uid[4] = uid_4;
    uid[5] = uid_5;
}
//--------------------------------------------------------------------------------
QString Device_base::get_str_uid(void)
{
    return QString("%1:%2:%3:%4:%5:%6")
            .arg(uid[0], 2, 16, QChar('0'))
            .arg(uid[1], 2, 16, QChar('0'))
            .arg(uid[2], 2, 16, QChar('0'))
            .arg(uid[3], 2, 16, QChar('0'))
            .arg(uid[4], 2, 16, QChar('0'))
            .arg(uid[5], 2, 16, QChar('0'))
            .toUpper();
}
//--------------------------------------------------------------------------------
