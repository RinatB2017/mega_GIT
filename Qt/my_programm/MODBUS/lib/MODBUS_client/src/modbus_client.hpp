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
    MODBUS_client(QWidget *parent = 0);
    ~MODBUS_client();

private slots:
    void readReady(void);
    void errorOccurred(QModbusDevice::Error);
    void stateChanged(QModbusDevice::State state);

    void connect_device(void);
    void disconnect_device(void);

    void connect_tcp_device(void);
    void disconnect_tcp_device(void);

    void refresh(void);

    void test_write_coils(void);
    void test_write_holding_registers(void);

    void test_read_discrete_inputs(void);
    void test_read_coils(void);
    void test_read_input_registers(void);
    void test_read_holding_registers(void);

private:
    Ui::MODBUS_client *ui;

    QModbusReply *lastRequest;
    QModbusClient *modbusDevice;

    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

    void init(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
