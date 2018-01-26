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
//--------------------------------------------------------------------------------
#include "modbus_client.hpp"
#include "ui_modbus_client.h"
//--------------------------------------------------------------------------------
#include <QModbusRtuSerialMaster>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MODBUS_client::MODBUS_client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MODBUS_client)
{
    ui->setupUi(this);

    //---
    if(parentWidget())
    {
        connect(this,   SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this,   SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this,   SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
        connect(this,   SIGNAL(trace(QString)), parentWidget(), SIGNAL(trace(QString)));
    }
    else
    {
        connect(this,   SIGNAL(info(QString)),  this,   SLOT(log(QString)));
        connect(this,   SIGNAL(debug(QString)), this,   SLOT(log(QString)));
        connect(this,   SIGNAL(error(QString)), this,   SLOT(log(QString)));
        connect(this,   SIGNAL(trace(QString)), this,   SLOT(log(QString)));
    }
    //---
    modbusDevice = new QModbusRtuSerialMaster(this);
    connect(modbusDevice,   SIGNAL(errorOccurred(QModbusDevice::Error)),    this,   SLOT(errorOccurred(QModbusDevice::Error)));
    connect(modbusDevice,   SIGNAL(stateChanged(QModbusDevice::State)),     this,   SLOT(stateChanged(QModbusDevice::State)));
    //---

    connect(ui->btn_connect,    SIGNAL(clicked(bool)),  this,   SLOT(connect_device()));
    connect(ui->btn_test,       SIGNAL(clicked(bool)),  this,   SLOT(test()));
    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    refresh();
}
//--------------------------------------------------------------------------------
MODBUS_client::~MODBUS_client()
{
    if(modbusDevice)
    {
        modbusDevice->disconnectDevice();
    }
    delete modbusDevice;
    delete ui;
}
//--------------------------------------------------------------------------------
void MODBUS_client::log(QString data)
{
#ifdef QT_DEBUG
    qDebug() << data;
#endif
}
//--------------------------------------------------------------------------------
void MODBUS_client::errorOccurred(QModbusDevice::Error)
{
    emit error(QString("MODBUS_client::errorOccurred %1").arg(modbusDevice->errorString()));
}
//--------------------------------------------------------------------------------
void MODBUS_client::stateChanged(QModbusDevice::State state)
{
    switch(state)
    {
    case QModbusDevice::UnconnectedState:   emit info("MODBUS_client::stateChanged UnconnectedState"); break;
    case QModbusDevice::ConnectingState:    emit info("MODBUS_client::stateChanged ConnectingState"); break;
    case QModbusDevice::ConnectedState:     emit info("MODBUS_client::stateChanged ConnectedState"); break;
    case QModbusDevice::ClosingState:       emit info("MODBUS_client::stateChanged ClosingState"); break;
    default:
        break;
    }
}
//--------------------------------------------------------------------------------
void MODBUS_client::test(void)
{
    emit debug("MODBUS_client::test");


    QModbusReply *reply = modbusDevice->sendReadRequest(readRequest(), 1);
    if(reply)
    {
        if (!reply->isFinished())
        {
            connect(reply, SIGNAL(finished()), this, SLOT(readReady()));
        }
        else
        {
            delete reply; // broadcast replies return immediately
        }
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::connect_device(void)
{
    emit info("MODBUS_client::connect_device");

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
    modbusDevice->setTimeout(1000);
    modbusDevice->setNumberOfRetries(1);
    //---
    if (!modbusDevice->connectDevice())
    {
        emit error(modbusDevice->errorString());
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::readReady(void)
{
    emit debug("MODBUS_client::readReady");

    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
    {
        emit error("reply is empty");
        return;
    }

    if (reply->error() == QModbusDevice::NoError)
    {
        QModbusDataUnit unit = reply->result();
        for (uint i=0; i<unit.valueCount(); i++)
        {
            QString entry = tr("Address: %1, Value: %2")
                    .arg(unit.startAddress())
                    .arg(QString::number(unit.value(i), unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
#if 0
            ui->writeTable->addItem(tr("Coils"),                QModbusDataUnit::Coils);
            ui->writeTable->addItem(tr("Discrete Inputs"),      QModbusDataUnit::DiscreteInputs);
            ui->writeTable->addItem(tr("Input Registers"),      QModbusDataUnit::InputRegisters);
            ui->writeTable->addItem(tr("Holding Registers"),    QModbusDataUnit::HoldingRegisters);
#endif
            emit info(entry);
        }
    }
    else
    {
        emit error(reply->errorString());
    }

    reply->deleteLater();
}
//--------------------------------------------------------------------------------
QModbusDataUnit MODBUS_client::readRequest() const
{
    QModbusDataUnit::RegisterType table = static_cast<QModbusDataUnit::RegisterType> (QModbusDataUnit::Coils);
    int startAddress = 0;
    int numberOfEntries = 1;

    return QModbusDataUnit(table, startAddress, numberOfEntries);
}
//--------------------------------------------------------------------------------
QModbusDataUnit MODBUS_client::writeRequest() const
{
    QModbusDataUnit::RegisterType table = static_cast<QModbusDataUnit::RegisterType> (QModbusDataUnit::Coils);
    int startAddress = 0;
    int numberOfEntries = 1;

    return QModbusDataUnit(table, startAddress, numberOfEntries);
}
//--------------------------------------------------------------------------------
void MODBUS_client::refresh(void)
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
