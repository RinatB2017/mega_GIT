/*********************************************************************************
**                                                                              **
**     Copyright (C) 2023                                                       **
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
#ifndef DAC32_HPP
#define DAC32_HPP
//--------------------------------------------------------------------------------
#include <QCoreApplication>
#include <QElapsedTimer>
//--------------------------------------------------------------------------------
#include "serialbox5_wo_form.hpp"
#include "switches_box.hpp"
#include "mywidget.hpp"
#include "packets.h"
//--------------------------------------------------------------------------------
class DAC32 : public MyWidget
{
    Q_OBJECT

public:
    explicit DAC32(QWidget *parent = nullptr);
    ~DAC32();

    uint8_t crc8(uint8_t *buf, uint8_t offset, uint8_t length);
    void f_dac_send(QByteArray data);

    bool port_is_open(void);

    bool find_device(void);
    void close_connect(void);

    void set_state_silence(bool state);

    bool f_GET_ID(void);
    bool f_GET_SWITCH(void);
    bool f_SET_SWITCH(uint8_t sw, uint8_t value);
    bool f_GET_SWITCHES(void);
    bool f_SET_SWITCHES(void);
    bool f_SET_SWITCHES(SWITCH values);
    bool f_GET_DAC_ZERO(uint16_t *value);
    bool f_SET_DAC_ZERO(uint16_t value);
    bool f_GET_DAC_LOW(uint16_t *value);
    bool f_SET_DAC_LOW(uint16_t value);
    bool f_GET_DAC_ADV(uint16_t *value);
    bool f_SET_DAC_ADV(uint16_t value);
    bool f_GET_DAC_ZERO_U0(uint16_t *value);
    bool f_SET_DAC_ZERO_U0(uint16_t value);

    bool f_get_switches(uint32_t *value);
    bool f_set_switches(uint32_t value);

signals:
    void update_ui(const QString &value);
    void update_ui(SWITCH sw);

    void port_is_active(bool);

private:
    QByteArray dac_data;

    SerialBox5_wo_form *serial = nullptr;

    bool data_is_ready = false;
    bool is_silence = false;

    void init(void);
    void read_data(QByteArray ba);

    void wait_msec(int timeout_msec, bool waiting = false);

    template<typename T>
    bool check_answer(int cmd);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif // DAC32_HPP
