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
#ifndef PET_DOCK_TESTBAR_HPP
#define PET_DOCK_TESTBAR_HPP
//--------------------------------------------------------------------------------
#include <stdint.h>
//--------------------------------------------------------------------------------
#include <QHostAddress>
#include <QComboBox>
#include <QToolButton>
#include <QToolBar>
//--------------------------------------------------------------------------------
#include "PET_device_base.hpp"
//--------------------------------------------------------------------------------
//class Packet_worker;
class QSpinBox;
//--------------------------------------------------------------------------------
class Dock_testbar : public PET_device_base
{
    Q_OBJECT

public:
    explicit Dock_testbar(QWidget *parent);
    virtual ~Dock_testbar();

    int get_PID(void);

private slots:
    void get_packet(QHostAddress host, quint16 port, QByteArray packet);
    void choice_test(void);
    void draw_data(QHostAddress host, quint16 port, QByteArray packet);
    bool check_packet(uint8_t cmd, QByteArray packet);

private:
    struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (Dock_testbar::*func)(void);
    };

    QComboBox *cb_test;
    QList<CMD> commands;

    bool is_ok;
    QByteArray clean_packet_buf;

    //---
    QSpinBox    *sb_device_address;
    QSpinBox    *sb_device_channel;
    QLineEdit   *le_server_address;
    QSpinBox    *sb_server_port;
    //---

    void init(void);
    void connect_log(void);

    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    bool wait_answer(void);
    void get_error(int error_code);

    bool f_PET_CMD_TEST(void);
    bool f_PET_CMD_DISCOVERY(void);
    bool f_PET_CMD_GET_STATE(void);
    bool f_PET_CMD_GET_CH_STATE(void);
    bool f_PET_CMD_SET_CONTROL(void);
    bool f_PET_CMD_GET_CONTROL(void);
    bool f_PET_CMD_SET_CH_CONTROL(void);
    bool f_PET_CMD_GET_CH_CONTROL(void);
    bool f_PET_CMD_RESET(void);
    bool f_PET_CMD_SHUTDOWN(void);
    bool f_PET_CMD_WAVE(void);
    bool f_PET_CMD_CONFIG_SAVE(void);
    bool f_PET_CMD_CONFIG_DEFAULT(void);
    bool f_PET_CMD_GET_DEVICE_INFO(void);
    bool f_PET_CMD_SET_CALIBRATION(void);
    bool f_PET_CMD_GET_CALIBRATION(void);
    bool f_PET_CMD_GET_NET_CONFIG(void);
    bool f_PET_CMD_SET_NET_CONFIG(void);
    bool f_PET_CMD_GET_STATISTICS(void);
    bool f_PET_CMD_GET_CH_STATISTICS(void);
    bool f_PET_CMD_UPDATE(void);

    bool f_PET_CMD_EVENT(void);
    bool f_PET_CMD_SET_EVENT_CONTROL(void);
    bool f_PET_CMD_GET_EVENT_CONTROL(void);

protected:
    virtual void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif
