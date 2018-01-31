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
#include <QSerialPortInfo>
#include <QHostAddress>
//--------------------------------------------------------------------------------
#include "modbus_server.hpp"
#include "ui_modbus_server.h"
//--------------------------------------------------------------------------------
#include "qhexedit.h"
#include "hexview16.hpp"
//--------------------------------------------------------------------------------
#include <QModbusTcpServer>
#include <QModbusRtuSerialSlave>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MODBUS_server::MODBUS_server(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MODBUS_server)
{
    init();
}
//--------------------------------------------------------------------------------
MODBUS_server::~MODBUS_server()
{
    if(he_discrete_inputs)  he_discrete_inputs->deleteLater();
    if(he_coils) he_coils->deleteLater();
    if(he_input_registers) he_input_registers->deleteLater();
    if(he_holding_registers) he_holding_registers->deleteLater();

    if(modbusDevice)
    {
        modbusDevice->disconnectDevice();
        modbusDevice->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void MODBUS_server::init(void)
{
    ui->setupUi(this);

    init_modbusDevice();
    init_tab_widget();

    refresh();
}
//--------------------------------------------------------------------------------
void MODBUS_server::init_modbusDevice(void)
{
    //---
    ui->le_server_host->setText("127.0.0.1:1500");
    //---
    connect(ui->btn_connect,    SIGNAL(clicked(bool)),  this,   SLOT(connect_device()));
    connect(ui->btn_disconnect, SIGNAL(clicked(bool)),  this,   SLOT(disconnect_device()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));
    //---
    connect(ui->btn_tcp_server_connect,     SIGNAL(clicked(bool)),  this,   SLOT(connect_tcp_device()));
    connect(ui->btn_tcp_server_disconnect,  SIGNAL(clicked(bool)),  this,   SLOT(disconnect_tcp_device()));
}
//--------------------------------------------------------------------------------
void MODBUS_server::init_tab_widget(void)
{
    while(ui->tabWidget->count())
    {
        ui->tabWidget->removeTab(0);
    }

    he_discrete_inputs = new HexView16(this);
    he_coils = new HexView16(this);
    he_input_registers = new HexView16(this);
    he_holding_registers = new HexView16(this);

    ui->tabWidget->addTab(he_discrete_inputs, "Discrete Inputs");
    ui->tabWidget->addTab(he_coils, "Coils");
    ui->tabWidget->addTab(he_input_registers, "Input Registers");
    ui->tabWidget->addTab(he_holding_registers, "Holding Registers");
}
//--------------------------------------------------------------------------------
void MODBUS_server::errorOccurred(QModbusDevice::Error)
{
    emit error(QString("MODBUS_client::errorOccurred %1").arg(modbusDevice->errorString()));
}
//--------------------------------------------------------------------------------
void MODBUS_server::stateChanged(QModbusDevice::State state)
{
    switch(state)
    {
    case QModbusDevice::UnconnectedState:   emit info("MODBUS_server::stateChanged UnconnectedState");  break;
    case QModbusDevice::ConnectingState:    emit info("MODBUS_server::stateChanged ConnectingState");   break;
    case QModbusDevice::ConnectedState:     emit info("MODBUS_server::stateChanged ConnectedState");    break;
    case QModbusDevice::ClosingState:       emit info("MODBUS_server::stateChanged ClosingState");      break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MODBUS_server::connect_device(void)
{
    emit info("MODBUS_server::connect_device");

    QString port = ui->cb_port->currentText();
    if(port.isEmpty())
    {
        emit error("port is empty!");
        return;
    }

    modbusDevice = new QModbusRtuSerialSlave(this);
    //---
    reg.insert(QModbusDataUnit::Coils,              { QModbusDataUnit::Coils,               0, 0xFFFF });   // однобитовый тип, доступен для чтения и записи.
    reg.insert(QModbusDataUnit::DiscreteInputs,     { QModbusDataUnit::DiscreteInputs,      0, 0xFFFF });   // однобитовый тип, доступен только для чтения.
    reg.insert(QModbusDataUnit::InputRegisters,     { QModbusDataUnit::InputRegisters,      0, 0xFFFF });   // 16-битовый знаковый или беззнаковый тип, доступен только для чтения.
    reg.insert(QModbusDataUnit::HoldingRegisters,   { QModbusDataUnit::HoldingRegisters,    0, 0xFFFF });   // 16-битовый знаковый или беззнаковый тип, доступен для чтения и записи.

    modbusDevice->setMap(reg);
    //---
    connect(modbusDevice,   SIGNAL(dataWritten(QModbusDataUnit::RegisterType,int,int)), this,   SLOT(updateWidgets(QModbusDataUnit::RegisterType,int,int)));
    connect(modbusDevice,   SIGNAL(errorOccurred(QModbusDevice::Error)),                this,   SLOT(errorOccurred(QModbusDevice::Error)));
    connect(modbusDevice,   SIGNAL(stateChanged(QModbusDevice::State)),                 this,   SLOT(stateChanged(QModbusDevice::State)));

    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,    port);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,      "0");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,    "9600");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,    "8");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,    "1");
    //---
    if (!modbusDevice->connectDevice())
    {
        emit error(modbusDevice->errorString());
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_server::updateWidgets(QModbusDataUnit::RegisterType table,
                                  int address,
                                  int size)
{
    quint16 value = 0;
    for (int i=0; i<size; ++i)
    {
        switch (table)
        {
        case QModbusDataUnit::Coils:
            modbusDevice->data(QModbusDataUnit::Coils, address + i, &value);
            he_coils->set(address+i, value);
            emit info(QString("Coils %1").arg(value));
            break;

        case QModbusDataUnit::HoldingRegisters:
            modbusDevice->data(QModbusDataUnit::HoldingRegisters, address + i, &value);
            emit info(QString("HoldingRegisters %1").arg(value));
            he_holding_registers->set(address+i, value);
            break;

        case QModbusDataUnit::DiscreteInputs:
            modbusDevice->data(QModbusDataUnit::DiscreteInputs, address + i, &value);
            emit info(QString("DiscreteInputs %1").arg(value));
            he_discrete_inputs->set(address+i, value);
            break;

        case QModbusDataUnit::InputRegisters:
            modbusDevice->data(QModbusDataUnit::InputRegisters, address + i, &value);
            emit info(QString("InputRegisters %1").arg(value));
            he_input_registers->set(address+i, value);
            break;

        default:
            emit error(QString("unknown table %1").arg(table));
            break;
        }
    }
}
//--------------------------------------------------------------------------------
void MODBUS_server::disconnect_device(void)
{
    if (modbusDevice->connectDevice())
    {
        modbusDevice->disconnect();
    }
}
//--------------------------------------------------------------------------------
void MODBUS_server::connect_tcp_device(void)
{
    emit info("MODBUS_server::connect_device");

    int serverAddress = 1;  //TODO

    QString port = ui->cb_port->currentText();
    if(port.isEmpty())
    {
        emit error("port is empty!");
        return;
    }

    const QUrl url = QUrl::fromUserInput(ui->le_server_host->text());
    emit info(QString("host: [%1:%2]")
              .arg(url.host())
              .arg(url.port()));

    //---
    modbusDevice = new QModbusTcpServer(this);
    //---
    reg.insert(QModbusDataUnit::Coils,              { QModbusDataUnit::Coils,               0, 0xFFFF });   // однобитовый тип, доступен для чтения и записи.
    reg.insert(QModbusDataUnit::DiscreteInputs,     { QModbusDataUnit::DiscreteInputs,      0, 0xFFFF });   // однобитовый тип, доступен только для чтения.
    reg.insert(QModbusDataUnit::InputRegisters,     { QModbusDataUnit::InputRegisters,      0, 0xFFFF });   // 16-битовый знаковый или беззнаковый тип, доступен только для чтения.
    reg.insert(QModbusDataUnit::HoldingRegisters,   { QModbusDataUnit::HoldingRegisters,    0, 0xFFFF });   // 16-битовый знаковый или беззнаковый тип, доступен для чтения и записи.

    modbusDevice->setMap(reg);
    //---
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter,    url.host());
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter,       url.port());
    modbusDevice->setServerAddress(serverAddress);
    //---
    if (!modbusDevice->connectDevice())
    {
        emit error(modbusDevice->errorString());
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_server::disconnect_tcp_device(void)
{
    if (modbusDevice->connectDevice())
    {
        modbusDevice->disconnect();
    }
}
//--------------------------------------------------------------------------------
void MODBUS_server::refresh(void)
{
    ui->cb_port->clear();
    foreach (QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
#ifdef Q_OS_LINUX
        ui->cb_port->addItem(port.systemLocation());
#endif
#ifdef Q_OS_WIN
        ui->cb_port->addItem(port.portName());
#endif
    }
}
//--------------------------------------------------------------------------------
void MODBUS_server::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
