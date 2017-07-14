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
#include <QDoubleSpinBox>
#include <QHostAddress>
#include <QSpinBox>
#include <QComboBox>
#include <QDateTime>
#include <QWidget>
#include <QString>
#include <QTime>
//--------------------------------------------------------------------------------
#include "petcat_options.hpp"
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "findpetbox.hpp"
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
PET_device_base::PET_device_base(QWidget *parent) :
    MyWidget(parent)
{
    comm = new PET_comm();
    connect(comm,   SIGNAL(info(QString)),      this,   SIGNAL(info(QString)));
    connect(comm,   SIGNAL(debug(QString)),     this,   SIGNAL(debug(QString)));
    connect(comm,   SIGNAL(error(QString)),     this,   SIGNAL(error(QString)));
    connect(comm,   SIGNAL(message(QString)),   this,   SIGNAL(message(QString)));

    if(parent)
    {
#ifdef NET_DEBUG
        connect(comm,   SIGNAL(send_net_log(QString)),      parent, SIGNAL(send_net_log(QString)));
        connect(comm,   SIGNAL(receive_net_log(QString)),   this,   SIGNAL(receive_net_log(QString)));
#endif
    }
    else
    {
        emit error("parentWidget not found");
    }

    addr = 0;
}
//--------------------------------------------------------------------------------
PET_device_base::~PET_device_base()
{
    if(comm)    comm->deleteLater();
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_PET_RESULT_string(int value)
{
    QString text;
    switch(value)
    {
    case PET_SUCCESS:           text = "Операция выполнена успешно";   break;
    case PET_ERR_NOT_IMPLEMENT: text = "Запрашиваемая команда не реализована";   break;
    case PET_ERR_BAD_FRAME:     text = "Неверный формат пакета";    break;
    case PET_ERR_BAD_CMD:       text = "Запрашиваемая команда не поддерживается";   break;
    case PET_ERR_BAD_ADDR:      text = "Запрашиваемый адрес устройства недоступен";   break;
    case PET_ERR_BAD_CHANNEL:   text = "Запрашиваемый номер канала обработки недоступен";   break;
    case PET_ERR_BAD_DATA:      text = "Передаваемые в запросе данные не корректны";   break;
    case PET_ERR_ACK:           text = "Устройство приняло запрос и обрабатывает его, но это требует много времени";   break;
    case PET_ERR_BUSY:          text = "Устройство занято обработкой ранее поступившей команды";   break;
    case PET_ERR_FAULT:         text = "Выявлена неустранимая ошибка выполнения команды";   break;
    case PET_ERR_TIMEOUT:       text = "Превышение времени выполнения операции";    break;
    case PET_ERR_NONE:          text = "Нет результата";    break;

    default:
        text = QString("unknown PET_RESULT %1").arg(value);
        break;
    }

    return text;
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_PET_VID_string(int value)
{
    QString text;
    switch(value)
    {
    case PET_VID_UNKNOWN:       text = "Не определён";  break;
    case PET_VID_NIITFA:        text = "АО \"НИИТФА\""; break;
    case PET_VID_RDCVECTOR:     text = "НТЦ \"Вектор\"";    break;

    default:
        text = QString("Неизвестный изготовитель (VID=0x%1)").arg(value);
        break;
    }

    return text;
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_PET_PID_string(int value)
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
        text = QString("Неизвестный тип устройства (PID=0x%1)").arg(value);
        break;
    }

    return text;
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_PET_UID_string(uint8_t uid[])
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
QString PET_device_base::get_device_type_long_str(int type)
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
        device_type = "Неизвестный тип устройства";
        break;
    }
    return device_type;
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_device_type_short_str(int type)
{
    QString device_type;
    switch(type)
    {
    case PET_PID_UNKNOWN:   device_type = "Тип устройства не определён";  break;
    case PET_PID_BOS:       device_type = "БОС";    break;
    case PET_PID_BRG:       device_type = "БРГ";    break;
    case PET_PID_SIM:       device_type = "СИМ";    break;
    case PET_PID_TD:        device_type = "БП";     break;
    case PET_PID_BRG_SIM:   device_type = "БРГ сим";    break;

    default:
        device_type = "Неизвестный тип устройства";
        break;
    }
    return device_type;
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_ip(uint8_t ip[])
{
    return QString("%1.%2.%3.%4")
            .arg(ip[0])
            .arg(ip[1])
            .arg(ip[2])
            .arg(ip[3]);
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_netmask(uint8_t netmask[])
{
    return QString("%1.%2.%3.%4")
            .arg(netmask[0])
            .arg(netmask[1])
            .arg(netmask[2])
            .arg(netmask[3]);
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_gateway(uint8_t gateway[])
{
    return QString("%1.%2.%3.%4")
            .arg(gateway[0])
            .arg(gateway[1])
            .arg(gateway[2])
            .arg(gateway[3]);
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_mac(uint8_t mac[])
{
    return QString("%1:%2:%3:%4:%5:%6")
            .arg(mac[5], 2, 16, QChar('0'))
            .arg(mac[4], 2, 16, QChar('0'))
            .arg(mac[3], 2, 16, QChar('0'))
            .arg(mac[2], 2, 16, QChar('0'))
            .arg(mac[1], 2, 16, QChar('0'))
            .arg(mac[0], 2, 16, QChar('0'))
            .toUpper();
}
//--------------------------------------------------------------------------------
QString PET_device_base::convert_timestamp_to_datatime_string(int32_t timestamp)
{
    if(timestamp == -1)
    {
        return "н/д";
    }

    QDate nullDate(1970, 1, 1);
    QDateTime nullTime = QDateTime(nullDate);
    QDateTime upTime = nullTime.addSecs(timestamp);

    QVariant days = nullTime.daysTo(upTime);
    QTime timeTime = upTime.time();
    QVariant hours = timeTime.hour();
    QVariant minutes = timeTime.minute();
    QVariant secs = timeTime.second();

    QString temp;

    if(days.toInt() > 0)
    {
        temp = QString("%1 дн. %2 час. %3 мин. %4 сек.")
                .arg(days.toInt())
                .arg(hours.toInt())
                .arg(minutes.toInt())
                .arg(secs.toInt());
        return temp;
    }
    if(hours.toInt() > 0)
    {
        temp = QString("%1 час. %2 мин. %3 сек.")
                .arg(hours.toInt())
                .arg(minutes.toInt())
                .arg(secs.toInt());
        return temp;
    }
    if(minutes.toInt() > 0)
    {
        temp = QString("%1 мин. %2 сек.")
                .arg(minutes.toInt())
                .arg(secs.toInt());
        return temp;
    }

    temp = QString("%1 сек.")
            .arg(secs.toInt());
    return temp;
}
//--------------------------------------------------------------------------------
QString PET_device_base::convert_version_to_string(uint16_t version)
{
    return QString("%1.%2")
            .arg((version >> 8) & 0x0F)
            .arg(version & 0x0F);

}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::set_comm_param(QHostAddress host, quint16 port)
{
    emit debug(QString("PET_device_base::set_comm_param(%1, %2)")
               .arg(host.toString())
               .arg(port));
    return comm->set_comm_param(host, port);
}
//--------------------------------------------------------------------------------
bool PET_device_base::bind(QHostAddress address, quint16 port)
{
    return comm->bind(address, port);
}
//--------------------------------------------------------------------------------
bool PET_device_base::bind(QHostAddress address)
{
    return comm->bind(address);
}
//--------------------------------------------------------------------------------
void PET_device_base::set_max_waiting_msec(uint16_t value)
{
    comm->set_max_waiting_msec(value);
}
//--------------------------------------------------------------------------------
uint16_t PET_device_base::get_max_waiting_msec(void)
{
    return comm->get_max_waiting_msec();
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::send_packet(QByteArray question, QByteArray *answer)
{
    Q_CHECK_PTR(answer);
    return comm->cmd(question, answer);
}
//--------------------------------------------------------------------------------
QString PET_device_base::get_last_error(void)
{
    return last_error;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::check_packet(QByteArray question, uint8_t cmd)
{
    if(question.length() < (int)sizeof(pet_frame))
    {
        return PET_ERR_BAD_FRAME;
    }
    //---
    pet_frame_t *frame = (pet_frame_t *)question.data();
    Q_CHECK_PTR(frame);
    bool f_error = (frame->cmd & PET_FRAME_CMD_FAULT_MSK);
    uint8_t f_cmd = (frame->cmd & ~PET_FRAME_CMD_FAULT_MSK);
    //---
    if(frame->addr != addr)
    {
        emit error(QString("check_packet [%1]").arg(question.toHex().data()));
        emit error(QString("send addr 0x%1").arg(addr, 0, 16));
        emit error(QString("get  addr 0x%1").arg(frame->addr, 0, 16));
        return PET_ERR_BAD_ADDR;
    }
    if(f_cmd != cmd)
    {
        emit error(QString("check_packet [%1]").arg(question.toHex().data()));
        emit error(QString("send cmd 0x%1").arg(cmd, 0, 16));
        emit error(QString("get  cmd 0x%1").arg(f_cmd, 0, 16));
        return PET_ERR_FAULT;
    }
    //---
    if(f_error)
    {
        pet_frame_error_t *err_frame = (pet_frame_error_t *)question.data();
        return (PET_RESULT)err_frame->result_cmd;
    }
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::reset(void)
{
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_RESET;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    return err;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::shutdown(void)
{
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_SHUTDOWN;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    return err;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::config_save(void)
{
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_CONFIG_SAVE;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    return err;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::config_default(void)
{
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_CONFIG_DEFAULT;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    return err;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::update(void *i_data, int *i_data_len,
                                   void *o_data, int *o_data_len)
{
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_UPDATE;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----

    question.clear();
    question.append((char *)&frame,     sizeof(frame));
    question.append((char *)&i_data,    *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::run(void)
{
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_RUN;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    return err;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::stop(void)
{
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_STOP;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    return err;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_get_test(void *i_data, int *i_data_len,
                                      void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_TEST;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = (int)*i_data_len;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data,  *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_set_ctrl(void *i_data, int *i_data_len,
                                      void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_SET_CONTROL;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data,  *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_get_ctrl(void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_GET_CONTROL;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_set_ch_ctrl(void *i_data, int *i_data_len,
                                         void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_SET_CH_CONTROL;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data, *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_get_ch_ctrl(void *i_data, int *i_data_len,
                                         void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_GET_CH_CONTROL;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data, *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
#ifdef QT_DEBUG
    //    QByteArray debug_ba;
    //    debug_ba.clear();
    //    debug_ba.append((char *)r_frame->data, len);
    //    emit debug(debug_ba.toHex());
#endif
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_get_state(void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_GET_STATE;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_get_ch_state(void *i_data, int *i_data_len,
                                          void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_GET_CH_STATE;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data, *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_set_calibration(void *i_data, int *i_data_len,
                                             void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_SET_CALIBRATION;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data, *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_get_calibration(void *i_data, int *i_data_len,
                                             void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_GET_CALIBRATION;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data, *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_get_device_info(void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_GET_DEVICE_INFO;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = 0;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_set_update(void *i_data, int *i_data_len,
                                        void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_UPDATE;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data, *i_data_len);
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
PET_RESULT PET_device_base::_get_wave(void *i_data, int *i_data_len,
                                      void *o_data, int *o_data_len)
{
    Q_CHECK_PTR(i_data);
    Q_CHECK_PTR(i_data_len);
    Q_CHECK_PTR(o_data);
    Q_CHECK_PTR(o_data_len);
    //---
    QByteArray question;
    QByteArray answer;
    //---
    uint8_t cmd = PET_CMD_WAVE;
    //---
    pet_frame_t frame;
    frame.addr = addr;
    frame.cmd = cmd;
    frame.len = *i_data_len;
    //----
    if(addr == PET_ADDR_ROOT)
    {
        return PET_ERR_BAD_CMD;
    }
    //----
    pet_wave_t *wave = (pet_wave_t *)i_data;
    emit debug(QString("addr %1").arg(addr));
    emit debug(QString("channel %1").arg(wave->channel));
    emit debug(QString("mode %1").arg(wave->mode));
    emit debug(QString("count %1").arg(wave->count));
    //----
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    question.append((char *)i_data, *i_data_len);
    //---
    //emit debug(QString("PET_device_base::get_wave %1")
    //           .arg(question.toHex().data()));
    //---
    PET_RESULT err = send_packet(question, &answer);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    err = check_packet(answer, cmd);
    if(err != PET_SUCCESS)
    {
        return err;
    }
    //---
    //emit debug(QString("answer [%1]").arg(answer.toHex().data()));
    //---
    pet_frame_t *r_frame = (pet_frame_t *)answer.data();
    int len = answer.length() - sizeof(frame);
    //---
    *o_data_len = len;
    memcpy((char *)o_data, (char *)r_frame->data, len);
    //---
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
//FIXME слишком много параметров
PET_RESULT PET_device_base::discovery(QWidget *mw,
                                      PET_comm *comm,
                                      QHostAddress host,
                                      quint16 port,
                                      uint8_t pet_addr,
                                      uint16_t max_timeout_msec,
                                      QList<pet_discovery_data_t> *l_discovery)
{
    Q_CHECK_PTR(comm);
    Q_CHECK_PTR(l_discovery);
    //---
    QByteArray question;
    //---
    pet_frame_t frame;
    frame.addr = pet_addr;
    frame.cmd = PET_CMD_DISCOVERY;
    frame.len = 0;
    //---
    PET_RESULT err = PET_SUCCESS;
    question.clear();
    question.append((char *)&frame, sizeof(frame));
    //---
    comm->set_max_waiting_msec(max_timeout_msec);
    err = comm->set_comm_param(host, port);
    if(err != PET_SUCCESS)
    {
        qDebug() << comm->get_last_error();
        return err;
    }
    //---
    err = comm->send_raw_data(question);
    if(err != PET_SUCCESS)
    {
        qDebug() << comm->get_last_error();
        return err;
    }
    //---
    QTime time;
    time.start();
    (*l_discovery).clear();
    while(time.elapsed() < max_timeout_msec)
    {
        QCoreApplication::processEvents();
        udp_data temp_data;
        bool ok = comm->get_remote_data(&temp_data);
        if(ok)
        {
            pet_frame_t *r_frame = (pet_frame_t *)temp_data.remote_data.data();
            bool flag_error = (r_frame->cmd & PET_FRAME_CMD_FAULT_MSK);
            if(!flag_error)
            {
                if(r_frame->cmd == PET_CMD_DISCOVERY)
                {
                    pet_discovery_info_t *d_info = (pet_discovery_info_t *)r_frame->data;

                    pet_discovery_data_t packet;
                    packet.host = temp_data.remote_host;
                    packet.port = temp_data.remote_port;
                    packet.addr = r_frame->addr;
                    packet.vid = d_info->vid;
                    packet.pid = d_info->pid;
                    for(unsigned int n=0; n<sizeof(d_info->uid); n++)
                    {
                        packet.uid[n] = d_info->uid[n];
                    }
                    //TODO
                    FindPetBox *w = dynamic_cast<FindPetBox *>(mw);
                    if(w)
                    {
                        w->add_pet_device(packet);
                    }
                    //---
                    (*l_discovery).append(packet);
                }
            }
        }
        else
        {
            //last_error = ba.toHex();
        }
    }
    if((*l_discovery).count() == 0)
    {
        return PET_ERR_TIMEOUT;
    }
    return PET_SUCCESS;
}
//--------------------------------------------------------------------------------
void PET_device_base::set_pet_addr(uint8_t value)
{
    addr = value;
}
//--------------------------------------------------------------------------------
uint8_t PET_device_base::get_pet_addr(void)
{
    return addr;
}
//--------------------------------------------------------------------------------
