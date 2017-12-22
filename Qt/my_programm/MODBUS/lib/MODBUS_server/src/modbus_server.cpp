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
#include <QDebug>
//--------------------------------------------------------------------------------
#include <QSerialPortInfo>
//--------------------------------------------------------------------------------
#include "modbus_server.hpp"
#include "ui_modbus_server.h"
//--------------------------------------------------------------------------------
#include <QModbusRtuSerialSlave>
//--------------------------------------------------------------------------------
MODBUS_server::MODBUS_server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MODBUS_server)
{
    ui->setupUi(this);

    //---
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
    //---
    modbusDevice = new QModbusRtuSerialSlave(this);
    //---
    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::Coils,              { QModbusDataUnit::Coils,               0, 10 });   // однобитовый тип, доступен для чтения и записи.
    reg.insert(QModbusDataUnit::DiscreteInputs,     { QModbusDataUnit::DiscreteInputs,      0, 10 });   // однобитовый тип, доступен только для чтения.
    reg.insert(QModbusDataUnit::InputRegisters,     { QModbusDataUnit::InputRegisters,      0, 10 });   // 16-битовый знаковый или беззнаковый тип, доступен только для чтения.
    reg.insert(QModbusDataUnit::HoldingRegisters,   { QModbusDataUnit::HoldingRegisters,    0, 10 });   // 16-битовый знаковый или беззнаковый тип, доступен для чтения и записи.

    modbusDevice->setMap(reg);
    //---
    connect(modbusDevice,   SIGNAL(dataWritten(QModbusDataUnit::RegisterType,int,int)), this,   SLOT(updateWidgets(QModbusDataUnit::RegisterType,int,int)));
    connect(modbusDevice,   SIGNAL(errorOccurred(QModbusDevice::Error)),                this,   SLOT(errorOccurred(QModbusDevice::Error)));
    connect(modbusDevice,   SIGNAL(stateChanged(QModbusDevice::State)),                 this,   SLOT(stateChanged(QModbusDevice::State)));
    //---
    for(int i=0; i<10; i++)
    {
        modbusDevice->setData(QModbusDataUnit::Coils,               i, 1);
        modbusDevice->setData(QModbusDataUnit::DiscreteInputs,      i, 1);
        modbusDevice->setData(QModbusDataUnit::InputRegisters,      i, i);
        //modbusDevice->setData(QModbusDataUnit::HoldingRegisters,    i, i);
    }
    //---

    connect(ui->btn_connect,    SIGNAL(clicked(bool)),  this,   SLOT(connect_device()));
    connect(ui->btn_test,       SIGNAL(clicked(bool)),  this,   SLOT(test()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    refresh();
}
//--------------------------------------------------------------------------------
MODBUS_server::~MODBUS_server()
{
    if(modbusDevice)
    {
        modbusDevice->disconnectDevice();
    }
    delete modbusDevice;
    delete ui;
}
//--------------------------------------------------------------------------------
void MODBUS_server::log(QString data)
{
    qDebug() << data;
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
void MODBUS_server::test(void)
{
    emit debug("MODBUS_server::test");
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
void MODBUS_server::updateWidgets(QModbusDataUnit::RegisterType table, int address, int size)
{
    //emit debug("MODBUS_server::updateWidgets");

    for (int i=0; i<size; ++i)
    {
        quint16 value;
        QString text;
        switch (table)
        {
        case QModbusDataUnit::Coils:
            modbusDevice->data(QModbusDataUnit::Coils, address + i, &value);
            //coilButtons.button(address + i)->setChecked(value);
            break;

        //case QModbusDataUnit::DiscreteInputs:
        //    modbusDevice->data(QModbusDataUnit::DiscreteInputs, address + i, &value);
        //    registers.value(QStringLiteral("DiscreteInputs_%1").arg(address + i))->setText(text.setNum(value, 16));
        //    break;

        //case QModbusDataUnit::InputRegisters:
        //    modbusDevice->data(QModbusDataUnit::InputRegisters, address + i, &value);
        //    registers.value(QStringLiteral("InputRegisters_%1").arg(address + i))->setText(text.setNum(value, 16));
        //    break;

        case QModbusDataUnit::HoldingRegisters:
            modbusDevice->data(QModbusDataUnit::HoldingRegisters, address + i, &value);
            registers.value(QStringLiteral("HoldingRegisters_%1").arg(address + i))->setText(text.setNum(value, 16));
            break;

        default:
            //emit error(QString("unknown table %1").arg(table));
            break;
        }
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
