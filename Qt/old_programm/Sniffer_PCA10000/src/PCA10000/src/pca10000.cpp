/*********************************************************************************
**                                                                              **
**     Copyright (C) 2014                                                       **
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
#include <QObject>
//--------------------------------------------------------------------------------
#include <QSerialPort>
#include <QSerialPortInfo>
//--------------------------------------------------------------------------------
#include "defines.hpp"
#include "pca10000.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
PCA10000::PCA10000(QObject *parent) :
    QObject(parent)
{
    init();
}
//--------------------------------------------------------------------------------
void PCA10000::init(void)
{
    connect(&serial, SIGNAL(readyRead()), this, SLOT(port_read()));
    //connect(&serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(port_error(QSerialPort::SerialPortError)));
#ifdef FAKE
    connect(&serial, SIGNAL(info(QString)),     this, SIGNAL(info(QString)));
    connect(&serial, SIGNAL(debug(QString)),    this, SIGNAL(debug(QString)));
    connect(&serial, SIGNAL(error(QString)),    this, SIGNAL(error(QString)));
    connect(&serial, SIGNAL(message(QString)),  this, SIGNAL(message(QString)));
#endif
}
//--------------------------------------------------------------------------------
void PCA10000::print_info(const QSerialPortInfo &port)
{
    emit info("------------------");
    emit info(QString("portName %1").arg(port.portName()));
    emit info(QString("systemLocation %1").arg(port.systemLocation()));
    emit info(QString("description %1").arg(port.description()));
    emit info(QString("manufacturer %1").arg(port.manufacturer()));
    //??? emit info(QString("serialNumber %1").arg(port.serialNumber()));
    emit info(QString("VID/PID 0x%1/0x%2")
              .arg(port.vendorIdentifier(), 0, 16)
              .arg(port.productIdentifier(), 0, 16));
    QList<qint32> rates = port.standardBaudRates();
    foreach(qint32 baud, rates)
    {
        emit error(QString("baud %1").arg(baud));
    }
}
//--------------------------------------------------------------------------------
bool PCA10000::find_device(void)
{
    int speed = 460800;
    if(serial.isOpen()) serial.close();

    foreach (const QSerialPortInfo &port, QSerialPortInfo::availablePorts())
    {
        if(!port.portName().contains("ACM")) continue;

        emit info(QString(tr("ищем PCA10000 на порту %1")).arg(port.portName()));
        serial.setPort(port);
        //emit info(QString("speed %1").arg(speed));
        //print_info(port);

        bool ok = serial.setBaudRate(speed);
        if(ok)
        {
            if(serial.open(QIODevice::ReadWrite))
            {
                emit info("OK");
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
void PCA10000::test(void)
{
    emit info("test");
    if(serial.isOpen() == false)
    {
        emit error("port not open!");
        return;
    }

    sniffer_data.clear();

    QByteArray ba;
    ba.clear();
    ba.append(PING_REQ);
    serial.write(ba);

    Sleeper::sleep(1);
    emit info(sniffer_data.toHex());
    emit info("OK");
}
//--------------------------------------------------------------------------------
void PCA10000::port_read(void)
{
    QByteArray ba = serial.readAll();
    if(ba.isEmpty()) return;

    sniffer_data.append(ba);
}
//--------------------------------------------------------------------------------
void PCA10000::port_error(QSerialPort::SerialPortError serial_error)
{
    switch(serial_error)
    {
    case QSerialPort::NoError:  break;
    case QSerialPort::DeviceNotFoundError:  emit error("Error: DeviceNotFoundError"); break;
    case QSerialPort::PermissionError:      emit error("Error: PermissionError"); break;
    case QSerialPort::OpenError:            emit error("Error: OpenError"); break;
    case QSerialPort::ParityError:          emit error("Error: ParityError"); break;
    case QSerialPort::FramingError:         emit error("Error: FramingError"); break;
    case QSerialPort::BreakConditionError:  emit error("Error: BreakConditionError"); break;
    case QSerialPort::WriteError:           emit error("Error: WriteError"); break;
    case QSerialPort::ReadError:            emit error("Error: ReadError"); break;
    case QSerialPort::ResourceError:        emit error("Error: ResourceError"); break;
    case QSerialPort::UnsupportedOperationError: emit error("Error: UnsupportedOperationError"); break;
    case QSerialPort::UnknownError:         emit error("Error: UnknownError"); break;
    case QSerialPort::TimeoutError:         emit error("Error: TimeoutError"); break;
    case QSerialPort::NotOpenError:         emit error("Error: NotOpenError"); break;
    default:
        emit error(QString("Unknown error %1").arg(serial_error));
        break;
    }
}
//--------------------------------------------------------------------------------
void PCA10000::encodeToSLIP(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::decodeFromSLIP(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::handlePacketHistory(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::sendPacket(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::sendScan(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::sendFollow(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::sendPingReq(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::sendTK(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::sendSwitchBaudRate(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::switchBaudRate(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::sendHopSequence(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::sendGoIdle(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::findSerialPort(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::readStaticHeader(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::writeStaticHeader(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::readDynamicHeader(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::writeDynamicHeader(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::readPayload(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::readFlags(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::getList(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::asString(void)
{

}
//--------------------------------------------------------------------------------
void PCA10000::validatePacketList(void)
{

}
//--------------------------------------------------------------------------------
