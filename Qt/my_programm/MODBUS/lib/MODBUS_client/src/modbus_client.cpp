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
#include <QModbusTcpClient>
#include <QModbusRtuSerialMaster>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MODBUS_client::MODBUS_client(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::MODBUS_client)
{
    init();;
}
//--------------------------------------------------------------------------------
MODBUS_client::~MODBUS_client()
{
    if(lastRequest)
    {
        lastRequest->deleteLater();
    }

    if(modbusDevice)
    {
        modbusDevice->disconnectDevice();
        modbusDevice->deleteLater();
    }

    delete ui;
}
//--------------------------------------------------------------------------------
void MODBUS_client::init(void)
{
    ui->setupUi(this);

    //---
    ui->le_client_host->setText("127.0.0.1:1500");
    //---
    ui->sb_start_address->setRange(0, 0xFFFF);
    //---

    connect(ui->btn_connect,    SIGNAL(clicked(bool)),  this,   SLOT(connect_device()));
    connect(ui->btn_disconnect, SIGNAL(clicked(bool)),  this,   SLOT(disconnect_device()));

    connect(ui->btn_test_read_discrete_inputs,      SIGNAL(clicked(bool)),  this,   SLOT(test_read_discrete_inputs()));
    connect(ui->btn_test_read_coils,                SIGNAL(clicked(bool)),  this,   SLOT(test_read_coils()));
    connect(ui->btn_test_read_input_registers,      SIGNAL(clicked(bool)),  this,   SLOT(test_read_input_registers()));
    connect(ui->btn_test_read_holding_registers,    SIGNAL(clicked(bool)),  this,   SLOT(test_read_holding_registers()));

    connect(ui->btn_test_write_coils,               SIGNAL(clicked(bool)),  this,   SLOT(test_write_coils()));
    connect(ui->btn_test_write_holding_registers,   SIGNAL(clicked(bool)),  this,   SLOT(test_write_holding_registers()));

    connect(ui->btn_refresh,    SIGNAL(clicked(bool)),  this,   SLOT(refresh()));

    connect(ui->btn_tcp_client_connect,     SIGNAL(clicked(bool)),  this,   SLOT(connect_tcp_device()));
    connect(ui->btn_tcp_client_disconnect,  SIGNAL(clicked(bool)),  this,   SLOT(disconnect_tcp_device()));

    refresh();
}
//--------------------------------------------------------------------------------
void MODBUS_client::errorOccurred(QModbusDevice::Error)
{
    emit error(QString("MODBUS_client::errorOccurred %1")
               .arg(modbusDevice->errorString()));
}
//--------------------------------------------------------------------------------
void MODBUS_client::stateChanged(QModbusDevice::State state)
{
    switch(state)
    {
    case QModbusDevice::UnconnectedState:   emit info("MODBUS_client::stateChanged UnconnectedState");  break;
    case QModbusDevice::ConnectingState:    emit info("MODBUS_client::stateChanged ConnectingState");   break;
    case QModbusDevice::ConnectedState:     emit info("MODBUS_client::stateChanged ConnectedState");    break;
    case QModbusDevice::ClosingState:       emit info("MODBUS_client::stateChanged ClosingState");      break;
    }
}
//--------------------------------------------------------------------------------
void MODBUS_client::test_write_coils(void)
{
    emit debug("MODBUS_client::test_write");

    int serverAddress = 1;  //TODO

    QVector<quint16> p(1);
    p[0]=static_cast<quint16>(ui->sb_write_value->value());

    QModbusDataUnit writeUnit;
    writeUnit.setRegisterType(QModbusDataUnit::Coils);
    writeUnit.setStartAddress(ui->sb_start_address->value());
    writeUnit.setValueCount(1);
    writeUnit.setValues(p);

    QModbusReply *w_reply = modbusDevice->sendWriteRequest(writeUnit, serverAddress);
    if(w_reply)
    {
        if (!w_reply->isFinished())
        {
            connect(w_reply,    SIGNAL(finished()), this,   SLOT(readReady()));
        }
        else
        {
            delete w_reply; // broadcast replies return immediately
        }
    }

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::test_write_holding_registers(void)
{
    emit debug("MODBUS_client::test_write");

    int serverAddress = 1;  //TODO

    QVector<quint16> p(1);
    p[0]=static_cast<quint16>(ui->sb_write_value->value());

    QModbusDataUnit writeUnit;
    writeUnit.setRegisterType(QModbusDataUnit::HoldingRegisters);
    writeUnit.setStartAddress(ui->sb_start_address->value());
    writeUnit.setValueCount(1);
    writeUnit.setValues(p);

    QModbusReply *w_reply = modbusDevice->sendWriteRequest(writeUnit, serverAddress);
    if(w_reply)
    {
        if (!w_reply->isFinished())
        {
            connect(w_reply,    SIGNAL(finished()), this,   SLOT(readReady()));
        }
        else
        {
            delete w_reply; // broadcast replies return immediately
        }
    }

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::test_read_discrete_inputs(void)
{
    emit debug("MODBUS_client::test_read");

    int serverAddress = 1;  //TODO

    QModbusDataUnit readUnit;
    readUnit.setRegisterType(QModbusDataUnit::DiscreteInputs);
    readUnit.setStartAddress(ui->sb_start_address->value());
    readUnit.setValueCount(1);

    QModbusReply *r_reply = modbusDevice->sendReadRequest(readUnit, serverAddress);
    if(r_reply)
    {
        if (!r_reply->isFinished())
        {
            connect(r_reply,    SIGNAL(finished()), this,   SLOT(readReady()));
        }
        else
        {
            delete r_reply; // broadcast replies return immediately
        }
    }

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::test_read_coils(void)
{
    emit debug("MODBUS_client::test_read");

    int serverAddress = 1;  //TODO

    QModbusDataUnit readUnit;
    readUnit.setRegisterType(QModbusDataUnit::Coils);
    readUnit.setStartAddress(ui->sb_start_address->value());
    readUnit.setValueCount(1);

    QModbusReply *r_reply = modbusDevice->sendReadRequest(readUnit, serverAddress);
    if(r_reply)
    {
        if (!r_reply->isFinished())
        {
            connect(r_reply,    SIGNAL(finished()), this,   SLOT(readReady()));
        }
        else
        {
            delete r_reply; // broadcast replies return immediately
        }
    }

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::test_read_input_registers(void)
{
    emit debug("MODBUS_client::test_read");

    int serverAddress = 1;  //TODO

    QModbusDataUnit readUnit;
    readUnit.setRegisterType(QModbusDataUnit::InputRegisters);
    readUnit.setStartAddress(ui->sb_start_address->value());
    readUnit.setValueCount(1);

    QModbusReply *r_reply = modbusDevice->sendReadRequest(readUnit, serverAddress);
    if(r_reply)
    {
        if (!r_reply->isFinished())
        {
            connect(r_reply,    SIGNAL(finished()), this,   SLOT(readReady()));
        }
        else
        {
            delete r_reply; // broadcast replies return immediately
        }
    }

    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::test_read_holding_registers(void)
{
    emit debug("MODBUS_client::test_read");

    int serverAddress = 1;  //TODO

    QModbusDataUnit readUnit;
    readUnit.setRegisterType(QModbusDataUnit::HoldingRegisters);
    readUnit.setStartAddress(ui->sb_start_address->value());
    readUnit.setValueCount(1);

    QModbusReply *r_reply = modbusDevice->sendReadRequest(readUnit, serverAddress);
    if(r_reply)
    {
        if (!r_reply->isFinished())
        {
            connect(r_reply,    SIGNAL(finished()), this,   SLOT(readReady()));
        }
        else
        {
            delete r_reply; // broadcast replies return immediately
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

    modbusDevice = new QModbusRtuSerialMaster(this);
    connect(modbusDevice,   SIGNAL(errorOccurred(QModbusDevice::Error)),    this,   SLOT(errorOccurred(QModbusDevice::Error)));
    connect(modbusDevice,   SIGNAL(stateChanged(QModbusDevice::State)),     this,   SLOT(stateChanged(QModbusDevice::State)));

    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,    port);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,      "0");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,    "9600");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,    "8");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,    "1");
    //---
    modbusDevice->setTimeout(1000);         //TODO
    modbusDevice->setNumberOfRetries(3);    //TODO
    //---
    if (!modbusDevice->connectDevice())
    {
        emit error(modbusDevice->errorString());
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::disconnect_device(void)
{
    if (modbusDevice->connectDevice())
    {
        disconnect(modbusDevice,   SIGNAL(errorOccurred(QModbusDevice::Error)),    this,   SLOT(errorOccurred(QModbusDevice::Error)));
        disconnect(modbusDevice,   SIGNAL(stateChanged(QModbusDevice::State)),     this,   SLOT(stateChanged(QModbusDevice::State)));
    }
}
//--------------------------------------------------------------------------------
void MODBUS_client::connect_tcp_device(void)
{
    emit info("MODBUS_client::connect_device");

    QString port = ui->cb_port->currentText();
    if(port.isEmpty())
    {
        emit error("port is empty!");
        return;
    }

    const QUrl url = QUrl::fromUserInput(ui->le_client_host->text());
    emit info(QString("host: [%1:%2]")
              .arg(url.host())
              .arg(url.port()));

    //---
    modbusDevice = new QModbusTcpClient(this);
    //---
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter,    url.host());
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter,       url.port());
    //---
    modbusDevice->setTimeout(1000);         //TODO
    modbusDevice->setNumberOfRetries(3);    //TODO
    //---
    if (!modbusDevice->connectDevice())
    {
        emit error(modbusDevice->errorString());
        return;
    }
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MODBUS_client::disconnect_tcp_device(void)
{
    if (modbusDevice->connectDevice())
    {
        disconnect(modbusDevice,   SIGNAL(errorOccurred(QModbusDevice::Error)),    this,   SLOT(errorOccurred(QModbusDevice::Error)));
        disconnect(modbusDevice,   SIGNAL(stateChanged(QModbusDevice::State)),     this,   SLOT(stateChanged(QModbusDevice::State)));
    }
}
//--------------------------------------------------------------------------------
//.arg(QString::number(unit.value(i), unit.registerType() <= QModbusDataUnit::Coils ? 10 : 16));
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
            QString entry = QString("Address: %1, Value: 0x%2")
                    .arg(unit.startAddress())
                    .arg(QString("%1").arg(unit.value(static_cast<int>(i)), 0, 16));
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

    return QModbusDataUnit(table,
                           startAddress,
                           static_cast<quint16>(numberOfEntries));
}
//--------------------------------------------------------------------------------
QModbusDataUnit MODBUS_client::writeRequest() const
{
    QModbusDataUnit::RegisterType table = static_cast<QModbusDataUnit::RegisterType> (QModbusDataUnit::Coils);
    int startAddress = 0;
    int numberOfEntries = 1;

    return QModbusDataUnit(table,
                           startAddress,
                           static_cast<quint16>(numberOfEntries));
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
void MODBUS_client::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MODBUS_client::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MODBUS_client::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MODBUS_client::save_setting(void)
{

}
//--------------------------------------------------------------------------------
