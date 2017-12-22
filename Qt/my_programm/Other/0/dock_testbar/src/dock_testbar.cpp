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
#include <QApplication>
#include <QHBoxLayout>
#include <QByteArray>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QTime>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "dock_testbar.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#include "pet_defines.hpp"
#include "pet.h"
//--------------------------------------------------------------------------------
Dock_testbar::Dock_testbar(QWidget *parent) :
    PET_device_base(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Dock_testbar::~Dock_testbar()
{

}
//--------------------------------------------------------------------------------
int Dock_testbar::get_PID(void)
{
    return PET_PID_SIM;
}
//--------------------------------------------------------------------------------
void Dock_testbar::init(void)
{
    connect_log();

    sb_device_address = new QSpinBox;
    sb_device_channel = new QSpinBox;
    le_server_address = new QLineEdit;
    sb_server_port = new QSpinBox;

    sb_device_address->setMinimum(0);   //включая адрес БОС
    sb_device_address->setMaximum(PET_ADDR_BRG_MAX);

    // каналы обработки 0-3 в случае с бос и 1-PET_ADDR_BRG_MAX в случае с брг
    sb_device_channel->setMinimum(0);
    sb_device_channel->setMaximum(PET_ADDR_BRG_MAX);

    sb_server_port->setMinimum(MIN_UDP_PORT);
    sb_server_port->setMaximum(MAX_UDP_PORT);
    sb_server_port->setValue(PET_NET_CMD_PORT_DEFAULT);

    le_server_address->setMinimumWidth(100);
    le_server_address->setText(DEFAULT_REMOTE_PET_ADDRESS);

    QHBoxLayout *hbox = new QHBoxLayout;

    hbox->addWidget(new QLabel("Server"));
    hbox->addWidget(le_server_address);
    hbox->addWidget(sb_server_port);
    hbox->addWidget(new QLabel("Device address"));
    hbox->addWidget(sb_device_address);
    hbox->addWidget(new QLabel("channel"));
    hbox->addWidget(sb_device_channel);

    commands.clear();
    CMD command;
    // DISCOVERY можно добавлять, не забывая, что запрос должен быть broadcast
    command.cmd = PET_CMD_TEST;                 command.cmd_text = "PET_CMD_TEST";              command.func = &Dock_testbar::f_PET_CMD_TEST;                 commands.append(command);
    command.cmd = PET_CMD_DISCOVERY;            command.cmd_text = "PET_CMD_DISCOVERY";         command.func = &Dock_testbar::f_PET_CMD_DISCOVERY;            commands.append(command);
    command.cmd = PET_CMD_GET_STATE;            command.cmd_text = "PET_CMD_GET_STATE";         command.func = &Dock_testbar::f_PET_CMD_GET_STATE;            commands.append(command);
    command.cmd = PET_CMD_GET_CH_STATE;         command.cmd_text = "PET_CMD_GET_CH_STATE";      command.func = &Dock_testbar::f_PET_CMD_GET_CH_STATE;         commands.append(command);
    command.cmd = PET_CMD_SET_CONTROL;          command.cmd_text = "PET_CMD_SET_CONTROL";       command.func = &Dock_testbar::f_PET_CMD_SET_CONTROL;          commands.append(command);
    command.cmd = PET_CMD_GET_CONTROL;          command.cmd_text = "PET_CMD_GET_CONTROL";       command.func = &Dock_testbar::f_PET_CMD_GET_CONTROL;          commands.append(command);
    command.cmd = PET_CMD_SET_CH_CONTROL;       command.cmd_text = "PET_CMD_SET_CH_CONTROL";    command.func = &Dock_testbar::f_PET_CMD_SET_CH_CONTROL;       commands.append(command);
    command.cmd = PET_CMD_GET_CH_CONTROL;       command.cmd_text = "PET_CMD_GET_CH_CONTROL";    command.func = &Dock_testbar::f_PET_CMD_GET_CH_CONTROL;       commands.append(command);
    command.cmd = PET_CMD_RESET;                command.cmd_text = "PET_CMD_RESET";             command.func = &Dock_testbar::f_PET_CMD_RESET;                commands.append(command);
    command.cmd = PET_CMD_SHUTDOWN;             command.cmd_text = "PET_CMD_SHUTDOWN";          command.func = &Dock_testbar::f_PET_CMD_SHUTDOWN;             commands.append(command);
    command.cmd = PET_CMD_WAVE;                 command.cmd_text = "PET_CMD_WAVE";              command.func = &Dock_testbar::f_PET_CMD_WAVE;                 commands.append(command);
    command.cmd = PET_CMD_CONFIG_SAVE;          command.cmd_text = "PET_CMD_CONFIG_SAVE";       command.func = &Dock_testbar::f_PET_CMD_CONFIG_SAVE;          commands.append(command);
    command.cmd = PET_CMD_CONFIG_DEFAULT;       command.cmd_text = "PET_CMD_CONFIG_DEFAULT";    command.func = &Dock_testbar::f_PET_CMD_CONFIG_DEFAULT;       commands.append(command);
    command.cmd = PET_CMD_GET_DEVICE_INFO;      command.cmd_text = "PET_CMD_GET_DEVICE_INFO";   command.func = &Dock_testbar::f_PET_CMD_GET_DEVICE_INFO;      commands.append(command);
    command.cmd = PET_CMD_UPDATE;               command.cmd_text = "PET_CMD_UPDATE";            command.func = &Dock_testbar::f_PET_CMD_UPDATE;               commands.append(command);
    command.cmd = PET_CMD_SET_CALIBRATION;      command.cmd_text = "PET_CMD_SET_CALIBRATION";   command.func = &Dock_testbar::f_PET_CMD_SET_CALIBRATION;      commands.append(command);
    command.cmd = PET_CMD_GET_CALIBRATION;      command.cmd_text = "PET_CMD_GET_CALIBRATION";   command.func = &Dock_testbar::f_PET_CMD_GET_CALIBRATION;      commands.append(command);
    command.cmd = PET_CMD_GET_NET_CONFIG;       command.cmd_text = "PET_CMD_GET_NET_CONFIG";    command.func = &Dock_testbar::f_PET_CMD_GET_NET_CONFIG;       commands.append(command);
    command.cmd = PET_CMD_SET_NET_CONFIG;       command.cmd_text = "PET_CMD_SET_NET_CONFIG";    command.func = &Dock_testbar::f_PET_CMD_SET_NET_CONFIG;       commands.append(command);
    command.cmd = PET_CMD_GET_STATISTICS;       command.cmd_text = "PET_CMD_GET_STATISTICS";    command.func = &Dock_testbar::f_PET_CMD_GET_STATISTICS;       commands.append(command);
    command.cmd = PET_CMD_GET_CH_STATISTICS;    command.cmd_text = "PET_CMD_GET_CH_STATISTICS"; command.func = &Dock_testbar::f_PET_CMD_GET_CH_STATISTICS;    commands.append(command);

    command.cmd = PET_CMD_EVENT;                command.cmd_text = "PET_CMD_EVENT";             command.func = &Dock_testbar::f_PET_CMD_EVENT;                commands.append(command);
    command.cmd = PET_CMD_SET_EVENT_CONTROL;    command.cmd_text = "PET_CMD_SET_EVENT_CONTROL"; command.func = &Dock_testbar::f_PET_CMD_SET_EVENT_CONTROL;    commands.append(command);
    command.cmd = PET_CMD_GET_EVENT_CONTROL;    command.cmd_text = "PET_CMD_GET_EVENT_CONTROL"; command.func = &Dock_testbar::f_PET_CMD_GET_EVENT_CONTROL;    commands.append(command);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(Qt::UserRole + command.cmd));
    }

    hbox->addWidget(cb_test);
    //---
    QToolButton *btn_choice_test = new QToolButton(this);
    btn_choice_test->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    btn_choice_test->setToolTip("doc_testbar_choice_test");
    btn_choice_test->setStatusTip("doc_testbar_choice_test");
    btn_choice_test->setObjectName("btn_choice_test");
    hbox->addWidget(btn_choice_test);
    //---
    hbox->addStretch(1);
    //---
    setLayout(hbox);

    connect(btn_choice_test, SIGNAL(clicked(bool)), this, SLOT(choice_test()));
}
//--------------------------------------------------------------------------------
void Dock_testbar::connect_log(void)
{
    if(parentWidget())
    {
        connect(this,   SIGNAL(info(QString)),      parentWidget(), SIGNAL(info(QString)));
        connect(this,   SIGNAL(debug(QString)),     parentWidget(), SIGNAL(debug(QString)));
        connect(this,   SIGNAL(error(QString)),     parentWidget(), SIGNAL(error(QString)));
        connect(this,   SIGNAL(message(QString)),   parentWidget(), SIGNAL(message(QString)));
    }
    else
    {
        connect(this,   SIGNAL(info(QString)),      this,   SLOT(log(QString)));
        connect(this,   SIGNAL(debug(QString)),     this,   SLOT(log(QString)));
        connect(this,   SIGNAL(error(QString)),     this,   SLOT(log(QString)));
        connect(this,   SIGNAL(message(QString)),   this,   SLOT(log(QString)));
    }

}
//--------------------------------------------------------------------------------
QToolButton *Dock_testbar::add_button(QToolBar *tool_bar,
                                      QToolButton *tool_button,
                                      QIcon icon,
                                      const QString &text,
                                      const QString &tool_tip)
{
    if(!tool_bar) return NULL;
    if(!tool_button) return NULL;

    tool_button->setIcon(icon);
    tool_button->setText(text);
    tool_button->setToolTip(tool_tip);
    tool_button->setObjectName(text);
    tool_bar->addWidget(tool_button);

    return tool_button;
}
//--------------------------------------------------------------------------------
void Dock_testbar::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok) - Qt::UserRole;
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (Dock_testbar::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void Dock_testbar::get_error(int error_code)
{
    emit error(QString("error code %1").arg(error_code));
}
//--------------------------------------------------------------------------------
void Dock_testbar::get_packet(QHostAddress host, quint16 port, QByteArray packet)
{
    if(QHostAddress(le_server_address->text()) != host)   return;
    if(sb_server_port->value() != port)   return;
    //---
    if(packet.isEmpty())
    {
        emit error("Dock_testbar::check_size_packet: packet is empty");
        return;
    }
    if(packet.length() < (int)sizeof(pet_frame_t))
    {
        emit error("Dock_testbar::check_size_packet: packet too small");
        return;
    }

    clean_packet_buf.clear();
    clean_packet_buf.append(packet);

    pet_frame_t *frame = (pet_frame_t *)packet.data();
    int len_packet = frame->len + sizeof(pet_frame_t);
    if(packet.length() != len_packet)
    {
        emit error("Dock_testbar::check_size_packet: bad len");
        emit error(QString("packet.length %1").arg(packet.length()));
        emit error(QString("len_packet %1").arg(len_packet));
        return;
    }
    //---
    is_ok = true;
}
//--------------------------------------------------------------------------------
void Dock_testbar::draw_data(QHostAddress host, quint16 port, QByteArray packet)
{
    pet_frame_t *r_frame = (pet_frame_t *)packet.data();
    pet_discovery_info_t *r_info = (pet_discovery_info_t *)r_frame->data;

    if(r_frame->cmd != PET_CMD_DISCOVERY)
    {
        return;
    }

    QString device = get_PET_PID_string(r_info->pid);

    QString uid;
    uid.append(QString("%1:").arg(r_info->uid[0], 2, 16, QChar('0')));
    uid.append(QString("%1:").arg(r_info->uid[1], 2, 16, QChar('0')));
    uid.append(QString("%1:").arg(r_info->uid[2], 2, 16, QChar('0')));
    uid.append(QString("%1:").arg(r_info->uid[3], 2, 16, QChar('0')));
    uid.append(QString("%1:").arg(r_info->uid[4], 2, 16, QChar('0')));
    uid.append(QString("%1").arg(r_info->uid[5],  2, 16, QChar('0')));

    //emit message(QString("Dock_testbar::draw_data packet [%1]").arg(packet.toHex().data()));
    emit info(QString("Dock_testbar::draw_data host %1").arg(host.toString()));
    emit info(QString("Dock_testbar::draw_data port %1").arg(port));
    emit info(QString("Dock_testbar::draw_data device %1").arg(device));
    emit info(QString("Dock_testbar::draw_data addr %1").arg(r_frame->addr));
    emit info(QString("Dock_testbar::draw_data VID %1").arg(get_PET_VID_string(r_info->vid)));
    emit info(QString("Dock_testbar::draw_data PID %1").arg(get_PET_PID_string(r_info->pid)));
    emit info(QString("Dock_testbar::draw_data uid %1").arg(uid));
}
//--------------------------------------------------------------------------------
bool Dock_testbar::wait_answer(void)
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
bool Dock_testbar::check_packet(uint8_t cmd, QByteArray packet)
{
    emit message(QString("received [%1]").arg(packet.toHex().data()));

    //emit debug(QString("check_size_packet: [%1]").arg(packet.toHex().data()));
    if(packet.isEmpty())
    {
        emit error("Dock_testbar::check_size_packet: packet is empty");
        return false;
    }
    if(packet.length() < (int)sizeof(pet_frame_t))
    {
        emit error("Dock_testbar::check_size_packet: packet too small");
        return false;
    }

    pet_frame_t *frame = (pet_frame_t *)packet.data();
    int len_packet = frame->len + sizeof(pet_frame_t);
    if(packet.length() != len_packet)
    {
        emit error("Dock_testbar::check_size_packet: bad len");
        return false;
    }

    uint8_t p_cmd = frame->cmd & 0x7F;
    if(p_cmd != cmd) return false;
    if(frame->cmd & PET_FRAME_CMD_FAULT_MSK)
    {
        emit error(get_PET_RESULT_string(frame->data[0]));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_TEST(void)
{
    emit debug("Dock_testbar::f_PET_CMD_TEST");
    QByteArray packet;
    packet.clear();

    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_frame_t frame;
    frame.addr = sb_device_address->value();
    frame.cmd = PET_CMD_TEST;
    frame.len = 0;
    //---
    packet.append((char *)&frame, sizeof(pet_frame_t));
    //---
    QByteArray output_packet;
    output_packet.clear();
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_DISCOVERY(void)
{
    emit debug("Dock_testbar::f_PET_CMD_DISCOVERY");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info(QString("PID %1").arg(get_PET_PID_string(o_info->pid)));
    emit info(QString("VID %1").arg(get_PET_VID_string(o_info->vid)));
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_STATE(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_STATE");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_bos_state_t state;
    int state_len = 0;
    //---
    PET_RESULT err = get_ch_state(&state, &state_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_CH_STATE(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_CH_STATE");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_bos_ch_state_t state;
    int state_len = 0;
    //---
    PET_RESULT err = get_ch_state(&state, &state_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_SET_CONTROL(void)
{
    emit debug("Dock_testbar::f_PET_CMD_SET_CONTROL");
    QByteArray packet;
    packet.clear();

    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_frame_t frame;
    frame.addr = sb_device_address->value();
    frame.cmd = PET_CMD_SET_CONTROL;
    frame.len = 0;
    //---
    packet.append((char *)&frame, sizeof(pet_frame_t));
    //---
    QByteArray output_packet;
    output_packet.clear();
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_CONTROL(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_CONTROL");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_SET_CH_CONTROL(void)
{
    emit debug("Dock_testbar::f_PET_CMD_SET_CH_CONTROL");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_CH_CONTROL(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_CH_CONTROL");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_bos_ch_ctrl_t ctrl;
    int ctrl_len = 0;
    //---
    PET_RESULT err = get_ch_ctrl(&ctrl, &ctrl_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_RESET(void)
{
    emit debug("Dock_testbar::f_PET_CMD_RESET");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_SHUTDOWN(void)
{
    emit debug("Dock_testbar::f_PET_CMD_SHUTDOWN");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_WAVE(void)
{
    emit debug("Dock_testbar::f_PET_CMD_WAVE");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_wave_t wave;
    int wave_len = 0;
    //---
    PET_RESULT err = get_wave(&wave, &wave_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_CONFIG_SAVE(void)
{
    emit debug("Dock_testbar::f_PET_CMD_CONFIG_SAVE");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_CONFIG_DEFAULT(void)
{
    emit debug("Dock_testbar::f_PET_CMD_CONFIG_DEFAULT");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    //---
    PET_RESULT err = send_packet(packet, &output_packet);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_DEVICE_INFO(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_DEVICE_INFO");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_device_info_t d_info;
    int d_info_len = 0;
    //---
    PET_RESULT err = get_device_info(&d_info, &d_info_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info(QString("VID %1").arg(get_PET_VID_string(d_info.vid)));
    emit info(QString("PID %1").arg(get_PET_PID_string(d_info.pid)));
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_UPDATE(void)
{
    emit debug("Dock_testbar::f_PET_CMD_UPDATE");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_update_data_t p_data;
    int p_data_len = 0;
    //---
    PET_RESULT err = set_update(&p_data, &p_data_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_SET_EVENT_CONTROL(void)
{
    emit debug("Dock_testbar::f_PET_CMD_SET_EVENT_CONTROL");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_event_control_t e_ctrl;
    int e_ctrl_len = 0;
    //---
    PET_RESULT err = set_event_ctrl(&e_ctrl, &e_ctrl_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info(QString("control 0x%1").arg(e_ctrl.control, 0, 16));
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_EVENT_CONTROL(void)
{
    emit debug("Dock_testbar::PET_CMD_GET_EVENT_CONTROL");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_event_control_t e_ctrl;
    int e_ctrl_len = 0;
    //---
    PET_RESULT err = get_event_ctrl(&e_ctrl, &e_ctrl_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info(QString("control 0x%1").arg(e_ctrl.control, 0, 16));
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_SET_CALIBRATION(void)
{
    emit debug("Dock_testbar::f_PET_CMD_SET_CALIBRATION");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_brg_ch_calibration_t calibration;
    int calibration_len = 0;
    //---
    PET_RESULT err = get_calibration(&calibration, &calibration_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_CALIBRATION(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_CALIBRATION");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_brg_ch_calibration_t calibration;
    int calibration_len = 0;
    //---
    PET_RESULT err = get_calibration(&calibration, &calibration_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_NET_CONFIG(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_NET_CONFIG");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_net_config_t n_config;
    int n_config_len = 0;
    //---
    PET_RESULT err = get_net_config(&n_config, &n_config_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_SET_NET_CONFIG(void)
{
    emit debug("Dock_testbar::f_PET_CMD_SET_NET_CONFIG");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_net_config_t n_config;
    int n_config_len = 0;
    //---
    PET_RESULT err = set_net_config(&n_config, &n_config_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_STATISTICS(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_STATISTICS");
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_brg_statistics_t statistics;
    int statistics_len = 0;
    //---
    PET_RESULT err = get_statistics(&statistics, &statistics_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info(QString("events_total %1").arg(statistics.events_total));
    emit info(QString("events_useful %1").arg(statistics.events_useful));
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_GET_CH_STATISTICS(void)
{
    emit debug("Dock_testbar::f_PET_CMD_GET_CH_STATISTICS");
    QByteArray packet;
    packet.clear();
    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_bos_ch_statistics_t statistics;
    int statistics_len = 0;
    //---
    PET_RESULT err = get_ch_statistics(&statistics, &statistics_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool Dock_testbar::f_PET_CMD_EVENT(void)
{
    emit debug("Dock_testbar::f_PET_CMD_EVENT");
    QByteArray packet;
    packet.clear();

    //---
    set_comm_param(QHostAddress(le_server_address->text()),
                   sb_server_port->value());
    //---
    pet_event_control event;
    int event_len = 0;
    //---
    PET_RESULT err = get_event(&event, &event_len);
    if(err != PET_SUCCESS)
    {
        emit error(get_PET_RESULT_string(err));
        return false;
    }
    //---
//    pet_frame_t *o_frame = (pet_frame_t *)output_packet.data();
//    int cnt = e->len / sizeof(pet_event_t);
//    emit info(QString("Получено %1 событий").arg(cnt));
    //---
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
void Dock_testbar::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        break;

    default:
        break;
    }
}
//--------------------------------------------------------------------------------
