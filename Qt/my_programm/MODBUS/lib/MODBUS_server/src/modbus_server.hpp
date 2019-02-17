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
#ifndef MODBUS_SERVER_HPP
#define MODBUS_SERVER_HPP
//--------------------------------------------------------------------------------
#include <QLineEdit>
#include <QWidget>
//--------------------------------------------------------------------------------
#include <QModbusDataUnit>
#include <QModbusDevice>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
namespace Ui {
class MODBUS_server;
}
//--------------------------------------------------------------------------------
class QModbusServer;
class QModbusReply;
class HexView16;
//class QHexEdit;
//--------------------------------------------------------------------------------
class MODBUS_server : public MyWidget
{
    Q_OBJECT

public:
    MODBUS_server(QWidget *parent = nullptr);
    ~MODBUS_server();

private slots:
    void errorOccurred(QModbusDevice::Error);
    void stateChanged(QModbusDevice::State state);
    void updateWidgets(QModbusDataUnit::RegisterType table,
                       int address,
                       int size);

    void connect_device(void);
    void disconnect_device(void);

    void connect_tcp_device(void);
    void disconnect_tcp_device(void);

    void refresh(void);

private:
    Ui::MODBUS_server *ui;

    QModbusReply  *lastRequest;
    QModbusServer *modbusDevice;

    QHash<QString, QLineEdit *> registers;

    QModbusDataUnitMap reg;

    HexView16 *he_discrete_inputs;
    HexView16 *he_coils;
    HexView16 *he_input_registers;
    HexView16 *he_holding_registers;

    void init(void);
    void init_modbusDevice(void);
    void init_tab_widget(void);
    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
