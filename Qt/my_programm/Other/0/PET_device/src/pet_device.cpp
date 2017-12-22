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
#include <QCoreApplication>
#include <QDebug>
#include <QTime>
//--------------------------------------------------------------------------------
#include "transceiver.hpp"
//--------------------------------------------------------------------------------
#include "pet_device.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
PET_device::PET_device(QWidget *parent) :
    QWidget(parent)
{
    init_client();
}
//--------------------------------------------------------------------------------
QString PET_device::get_PET_RESULT_string(int value)
{
    QString text;
    switch(value)
    {
    case PET_SUCCESS:       text = "Операция выполнена успешно";   break;
    case PET_NOT_IMPLEMENT: text = "PET_NOT_IMPLEMENT: Запрашиваемая команда не реализована";   break;
    case PET_BAD_FRAME:     text = "PET_BAD_FRAME: Неверный формат пакета";    break;
    case PET_BAD_CMD:       text = "PET_BAD_CMD: Запрашиваемая команда не поддерживается";   break;
    case PET_BAD_ADDR:      text = "PET_BAD_ADDR: Запрашиваемый адрес устройства недоступен";   break;
    case PET_BAD_CHANNEL:   text = "PET_BAD_CHANNEL: Запрашиваемый номер канала обработки недоступен";   break;
    case PET_BAD_DATA:      text = "PET_BAD_DATA: Передаваемые в запросе данные не корректны";   break;
    case PET_ACK:           text = "PET_ACK: Устройство приняло запрос и обрабатывает его, но это требует много времени";   break;
    case PET_BUSY:          text = "PET_BUSY: Устройство занято обработкой ранее поступившей команды";   break;
    case PET_FAULT:         text = "PET_FAULT: Выявлена неустранимая ошибка выполнения команды";   break;
    case PET_TIMEOUT:       text = "PET_TIMEOUT: Превышение времени выполнения операции";    break;
    case PET_NONE:          text = "PET_NONE: Нет результата";    break;

    default:
        text = QString("unknown PET_RESULT %1").arg(value);
        break;
    }

    return text;
}
//--------------------------------------------------------------------------------
QString PET_device::get_PET_VID_string(int value)
{
    QString text;
    switch(value)
    {
    case PET_VID_UNKNOWN:       text = "Не определён";  break;
    case PET_VID_NIITFA:        text = "АО \"НИИТФА\""; break;
    case PET_VID_RDCVECTOR:     text = "НТЦ \"Вектор\"";    break;

    default:
        text = QString("unknown PIT_VID %1").arg(value);
        break;
    }

    return text;
}
//--------------------------------------------------------------------------------
QString PET_device::get_PET_PID_string(int value)
{
    QString text;
    switch(value)
    {
    case PET_PID_UNKNOWN:   text = "Тип устройства не определён";   break;
    case PET_PID_BOS:       text = "Блок обработки событий ВМСТ.418258.001";   break;
    case PET_PID_BRG:       text = "Блок регистрации гамма-квантов БРГ-1 ВШЛК.418257.001";   break;
    case PET_PID_SIM:       text = "Симулятор ПЭТ"; break;
    case PET_PID_TD:        text = "Блок проверочный ВМСТ.468993.001";   break;
    case PET_PID_BRG_SIM:   text = "Программный симулятор блока БРГ";   break;
    default:
        text = QString("unknown PET_PID %1").arg(value);
        break;
    }

    return text;
}
//--------------------------------------------------------------------------------
QString PET_device::get_device_type_long_str(int type)
{
    QString device_type;
    switch(type)
    {
    case PET_PID_UNKNOWN:   device_type = "Тип устройства не определён";  break;
    case PET_PID_BOS:       device_type = "Блок обработки событий ВМСТ.418258.001";  break;
    case PET_PID_BRG:       device_type = "Блок регистрации гамма-квантов БРГ-1 ВШЛК.418257.001";  break;
    case PET_PID_SIM:       device_type = "Симулятор ПЭТ";  break;
    case PET_PID_TD:        device_type = "Блок проверочный ВМСТ.468993.001";  break;
    case PET_PID_BRG_SIM:   device_type = "Программный симулятор блока БРГ";  break;

    default:
        device_type = QString("Неизвестный тип устройства 0x%1").arg(type, 0, 16);
        break;
    }
    return device_type;
}
//--------------------------------------------------------------------------------
QString PET_device::get_device_type_short_str(int type)
{
    QString device_type;
    switch(type)
    {
    case PET_PID_UNKNOWN:    device_type = "Тип устройства не определён";  break;
    case PET_PID_BOS:        device_type = "БОС";  break;
    case PET_PID_BRG:        device_type = "БРГ";  break;
    case PET_PID_TD:         device_type = "БП";  break;
    case PET_PID_BRG_SIM:    device_type = "БРГ sim.";  break;

    default:
        device_type = QString("Неизвестный тип устройства 0x%1").arg(type, 0, 16);
        break;
    }
    return device_type;
}
//--------------------------------------------------------------------------------
void PET_device::log(const QString text)
{
    qDebug() << text;
}
//--------------------------------------------------------------------------------
void PET_device::init_client(void)
{
    //---
    p_pet_config.host = QHostAddress::AnyIPv4;
    p_pet_config.port = PET_NET_CMD_PORT_DEFAULT;
    p_pet_config.addr = 0;
    memset((char *)&p_pet_config.pet_info, 0, sizeof(pet_discovery_info_t));
    //---

    transceiver = new Transceiver(this);
    connect(transceiver,    SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(transceiver,    SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(transceiver,    SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(transceiver,    SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));

    connect(transceiver,    SIGNAL(get_packet(QHostAddress,quint16,QByteArray)),
            this,           SLOT(get_packet(QHostAddress,quint16,QByteArray)));

    transceiver->bind(p_pet_config.host);
}
//--------------------------------------------------------------------------------
void PET_device::get_packet(QHostAddress host,
                            quint16 port,
                            QByteArray packet)
{
    //emit message(QString("get_packet host [%1]").arg(host.toString()));
    //emit message(QString("PET_device::get_packet received packet [%1]").arg(packet.toHex().data()));

    if(is_ok)
    {
        return;
    }
    if(host != p_pet_config.host)
    {
        emit debug(QString("PET_device::get_packet: host %1 != %2")
                   .arg(host.toString())
                   .arg(p_pet_config.host.toString()));
        return;
    }
    if(port != p_pet_config.port)
    {
        emit debug(QString("PET_device::get_packet: port %1 != %2")
                   .arg(port)
                   .arg(p_pet_config.port));
        return;
    }

    receied_packet.clear();
    receied_packet.append(packet);
    is_ok = true;
}
//--------------------------------------------------------------------------------
QByteArray PET_device::get_received_packet(void)
{
    return receied_packet;
}
//--------------------------------------------------------------------------------
bool PET_device::wait_answer(void)
{
    QTime time;
    time.start();
    while(time.elapsed() < MAX_TIMEOUT_MSEC)
    {
        QCoreApplication::processEvents();
        if(is_ok)
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool PET_device::check_packet(uint8_t cmd, QByteArray packet)
{
    //emit debug(QString("check_size_packet: [%1]").arg(packet.toHex().data()));
    if(packet.isEmpty())
    {
        emit error("PET_device::check_size_packet: packet is empty");
        return false;
    }
    //---
    if(packet.length() < (int)sizeof(pet_frame_t))
    {
        emit error("PET_device::check_size_packet: packet too small");
        return false;
    }
    //---
    pet_frame_t *frame = (pet_frame_t *)packet.data();
    //---
    if((int)packet.length() != ((int)sizeof(pet_frame_t) + (int)frame->len))
    {
        emit error("PET_device::check_size_packet: packet bad data");
        emit error(QString("packet.length() %1").arg(packet.length()));
        emit error(QString("sizeof(pet_frame_t) %1")
                   .arg(sizeof(pet_frame_t)));
        emit error(QString("frame->len %1")
                   .arg(frame->len));
        return false;
    }
    //---
    uint8_t p_cmd = frame->cmd & 0x7F;
    if(p_cmd != cmd)
    {
        emit error(QString("p_cmd 0x%1 != cmd 0x%2")
                   .arg(p_cmd, 0, 16)
                   .arg(cmd, 0, 16));
        return false;
    }
    if(frame->cmd & PET_FRAME_CMD_FAULT_MSK)
    {
        emit error(get_PET_RESULT_string(frame->data[0]));
        return false;
    }
    //---
#if 0
    //FIXME
    int len_packet = sizeof(pet_frame_t);
    pet_wave_t *o_wave = (pet_wave_t *)frame->data;
    switch(frame->cmd)
    {
    case PET_CMD_TEST:              break;
    case PET_CMD_RESET:             break;
    case PET_CMD_SHUTDOWN:          break;

    case PET_CMD_WAVE:
        len_packet += sizeof(pet_wave);
        len_packet += o_wave->count * sizeof(pet_wave_point_t);
        break;

    case PET_CMD_DISCOVERY:
        len_packet += sizeof(pet_discovery_info_t);
        break;

    case PET_CMD_CONFIG_SAVE:       break;
    case PET_CMD_CONFIG_DEFAULT:    break;

    case PET_CMD_GET_DEVICE_INFO:
        len_packet += sizeof(pet_device_info_t);
        break;

    case PET_CMD_UPDATE:            break;

    case PET_CMD_SET_CALIBRATION:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_calibration_t);
        else
            len_packet += sizeof(pet_brg_calibration_t);
        break;

    case PET_CMD_GET_CALIBRATION:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_calibration_t);
        else
            len_packet += sizeof(pet_brg_calibration_t);
        break;

    case PET_CMD_GET_NET_CONFIG:
        len_packet += sizeof(pet_net_config);
        break;

    case PET_CMD_SET_NET_CONFIG:
        len_packet += sizeof(pet_net_config);
        break;

    case PET_CMD_GET_STATISTICS:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_statistics_t);
        else
            len_packet += sizeof(pet_brg_statistics_t);
        break;

    case PET_CMD_SET_CONTROL:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_ctrl_t);
        else
            len_packet += sizeof(pet_brg_ctrl_t);
        break;

    case PET_CMD_GET_CONTROL:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_ctrl_t);
        else
            len_packet += sizeof(pet_brg_ctrl_t);
        break;

    case PET_CMD_GET_STATE:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_state_t);
        else
            len_packet += sizeof(pet_brg_state_t);
        break;

    case PET_CMD_GET_CH_STATE:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_ch_state_t);
        else
            len_packet += sizeof(pet_brg_ch_state_t);
        break;

    case PET_CMD_GET_CH_CONTROL:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_ch_ctrl_t);
        else
            len_packet += sizeof(pet_brg_ch_ctrl_t);
        break;

    case PET_CMD_SET_CH_CONTROL:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_ch_ctrl_t);
        else
            len_packet += sizeof(pet_brg_ch_ctrl_t);
        break;

    case PET_CMD_GET_CH_STATISTICS:
        if(frame->addr == PET_ADDR_ROOT)
            len_packet += sizeof(pet_bos_ch_statistics_t);
        else
            len_packet += sizeof(pet_brg_ch_statistics_t);
        break;

    case PET_CMD_EVENT:
        len_packet += sizeof(pet_event);
        break;

    case PET_CMD_SET_EVENT_CONTROL:
        len_packet += sizeof(pet_event_control);
        break;

    case PET_CMD_GET_EVENT_CONTROL:
        len_packet += sizeof(pet_event_control);
        break;

    default:
        break;
    }
    if(packet.length() != len_packet)
    {
        emit error("PET_device::check_size_packet: bad len");
        return false;
    }
#endif
    //---
    return true;
}
//--------------------------------------------------------------------------------
bool PET_device::send_packet(QByteArray input_packet,
                             QByteArray *output_packet)
{
    //emit info("send_packet");
    //---
    bool ok = false;
    //---
    is_ok = false;
    //    emit info(QString("send_packet %1:%2")
    //              .arg(p_pet_config.host.toString())
    //              .arg(p_pet_config.port));
    ok = transceiver->send_packet(p_pet_config.host,
                                  p_pet_config.port,
                                  input_packet);
    if(!ok)
    {
        emit error(transceiver->get_last_error());
        return false;
    }
    //---
    is_ok = false;
    wait_answer();
    if(!is_ok)
    {
        emit error("wait_answer: FAIL");
        return false;
    }
    //---
    pet_frame_t *frame = (pet_frame_t *)input_packet.data();
    ok = check_packet(frame->cmd, receied_packet);
    if(!ok)
    {
        emit error("check_packet: FAIL");
        return false;
    }
    //---
    (*output_packet).append(receied_packet);

    return true;
}
//--------------------------------------------------------------------------------
void PET_device::set_param(PET_device_remote_t param)
{
    p_pet_config = param;
}
//--------------------------------------------------------------------------------
bool PET_device::bind(quint16 port)
{
    bool ok = false;
    ok = transceiver->bind(port);
    if(!ok)
    {
        emit error(transceiver->get_last_error());
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
