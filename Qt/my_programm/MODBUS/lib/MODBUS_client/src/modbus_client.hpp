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
#ifndef MODBUS_CLIENT_HPP
#define MODBUS_CLIENT_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QUrl>
//--------------------------------------------------------------------------------
#include <QModbusDataUnit>
#include <QModbusDevice>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
    class MODBUS_client;
}
//--------------------------------------------------------------------------------
class QModbusClient;
class QModbusReply;
//--------------------------------------------------------------------------------
class MODBUS_client : public MyWidget
{
    Q_OBJECT

public:
    MODBUS_client(QWidget *parent = nullptr);
    virtual ~MODBUS_client();

private slots:
    void readReady();
    void errorOccurred(QModbusDevice::Error);
    void stateChanged(QModbusDevice::State state);

    void connect_device();
    void disconnect_device();

    void connect_tcp_device();
    void disconnect_tcp_device();

    void refresh();

    void test_write_coils();
    void test_write_holding_registers();

    void test_read_discrete_inputs();
    void test_read_coils();
    void test_read_input_registers();
    void test_read_holding_registers();

private:
    Ui::MODBUS_client *ui;

    QModbusReply *lastRequest;
    QModbusClient *modbusDevice;

    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

    void init();

    void updateText();
    bool programm_is_exit();
    void load_setting();
    void save_setting();
};
//--------------------------------------------------------------------------------
#endif
