/*********************************************************************************
**                                                                              **
**     Copyright (C) 2012                                                       **
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
#include <QCoreApplication>
#include <QStringList>
#include <QByteArray>
#include <QIODevice>
#include <QDebug>
#include <QTime>
//--------------------------------------------------------------------------------
#include "emulator_v786_2.hpp"
#include "fakeserial.hpp"
//--------------------------------------------------------------------------------
FakeSerial::FakeSerial(QWidget *parent) :
    QWidget(parent),
    emulator_v786_2(0)
{
    init();
}
//--------------------------------------------------------------------------------
void FakeSerial::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void FakeSerial::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void FakeSerial::init(void)
{
    connect_log();

    emulator_v786_2 = new Emulator_V786_2(0,
                                          210,
                                          2013,
                                          QString(" 0.0000"),
                                          12000,
                                          1000,
                                          this);

    emitStatus = false;
    state = false;

    list_baudRate.clear();
    list_baudRate.append("Undefined baud");
    list_baudRate.append("50 baud");
    list_baudRate.append("75 baud");
    list_baudRate.append("110 baud");
    list_baudRate.append("134 baud");
    list_baudRate.append("150 baud");
    list_baudRate.append("200 baud");
    list_baudRate.append("300 baud");
    list_baudRate.append("600 baud");
    list_baudRate.append("1200 baud");
    list_baudRate.append("1800 baud");
    list_baudRate.append("2400 baud");
    list_baudRate.append("4800 baud");
    list_baudRate.append("9600 baud");
    list_baudRate.append("14400 baud");
    list_baudRate.append("19200 baud");
    list_baudRate.append("38400 baud");
    list_baudRate.append("56000 baud");
    list_baudRate.append("57600 baud");
    list_baudRate.append("76800 baud");
    list_baudRate.append("115200 baud");
    list_baudRate.append("128000 baud");
    list_baudRate.append("230400 baud");
    list_baudRate.append("256000 baud");
    list_baudRate.append("460800 baud");
    list_baudRate.append("500000 baud");
    list_baudRate.append("576000 baud");
    list_baudRate.append("921600 baud");
    list_baudRate.append("1000000 baud");
    list_baudRate.append("1152000 baud");
    list_baudRate.append("1500000 baud");
    list_baudRate.append("2000000 baud");
    list_baudRate.append("2500000 baud");
    list_baudRate.append("3000000 baud");
    list_baudRate.append("3500000 baud");
    list_baudRate.append("4000000 baud");

    list_dataBits.clear();
    list_dataBits.append("Undefined data bits");
    list_dataBits.append("5 bit");
    list_dataBits.append("6 bit");
    list_dataBits.append("7 bit");
    list_dataBits.append("8 bit");

    list_parity.clear();
    list_parity.append("Undefined parity");
    list_parity.append("None");
    list_parity.append("Odd");
    list_parity.append("Even");
    list_parity.append("Mark");
    list_parity.append("Space");

    list_stopBits.clear();
    list_stopBits.append("Unsefined stop bits");
    list_stopBits.append("One");
    list_stopBits.append("Half");
    list_stopBits.append("Two");

    list_flowControl.clear();
    list_flowControl.append("Undefined flow");
    list_flowControl.append("Disable");
    list_flowControl.append("Hardware");
    list_flowControl.append("Xon/Xoff");

    default_deviceName="S0";
    default_baudRate="9600 baud";
    default_dataBits="8 bit";
    default_parity="None";
    default_stopBits="One";
    default_flowControl="Disable";
}
//--------------------------------------------------------------------------------
QStringList FakeSerial::listBaudRate(void)
{
    return list_baudRate;
}
//--------------------------------------------------------------------------------
QStringList FakeSerial::listDataBits(void)
{
    return list_dataBits;
}
//--------------------------------------------------------------------------------
QStringList FakeSerial::listParity(void)
{
    return list_parity;
}
//--------------------------------------------------------------------------------
QStringList FakeSerial::listStopBits(void)
{
    return list_stopBits;
}
//--------------------------------------------------------------------------------
QStringList FakeSerial::listFlowControl(void)
{
    return list_flowControl;
}
//--------------------------------------------------------------------------------
QString FakeSerial::baudRate(void)
{
    return string_baudRate;
}
//--------------------------------------------------------------------------------
QString FakeSerial::dataBits(void)
{
    return string_dataBits;
}
//--------------------------------------------------------------------------------
QString FakeSerial::parity(void)
{
    return string_parity;
}
//--------------------------------------------------------------------------------
QString FakeSerial::stopBits(void)
{
    return string_stopBits;
}
//--------------------------------------------------------------------------------
QString FakeSerial::flowControl(void)
{
    return string_flowControl;
}
//--------------------------------------------------------------------------------
void FakeSerial::setDeviceName(const QString &text)
{
    foreach (QString name, list_deviceName)
    {
        if(name == text)
        {
            string_deviceName = text;
            //emit info(QString("setDeviceName(%1)").arg(text));
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void FakeSerial::setBaudRate(const QString &text)
{
    foreach (QString name, list_baudRate)
    {
        if(name == text)
        {
            string_baudRate = text;
            //emit info(QString("setBaudRate(%1)").arg(text));
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void FakeSerial::setDataBits(const QString &text)
{
    foreach (QString name, list_dataBits)
    {
        if(name == text)
        {
            string_dataBits = text;
            //emit info(QString("setDataBits(%1)").arg(text));
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void FakeSerial::setParity(const QString &text)
{
    foreach (QString name, list_parity)
    {
        if(name == text)
        {
            string_parity = text;
            //emit info(QString("setParity(%1)").arg(text));
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void FakeSerial::setStopBits(const QString &text)
{
    foreach (QString name, list_stopBits)
    {
        if(name == text)
        {
            string_stopBits = text;
            //emit info(QString("setStopBits(%1)").arg(text));
            return;
        }
    }
}
//--------------------------------------------------------------------------------
void FakeSerial::setFlowControl(const QString &text)
{
    foreach (QString name, list_flowControl)
    {
        if(name == text)
        {
            string_flowControl = text;
            //emit info(QString("setFlowControl(%1)").arg(text));
            return;
        }
    }
}
//--------------------------------------------------------------------------------
bool FakeSerial::isOpen(void)
{
    return state;
}
//--------------------------------------------------------------------------------
void FakeSerial::enableEmitStatus(bool state)
{
    emitStatus = state;
}
//--------------------------------------------------------------------------------
bool FakeSerial::open(QIODevice::OpenMode)
{
    emit signalStatus("FakeSerial open", QDateTime::currentDateTime());

    setBaudRate(default_baudRate);
    setDataBits(default_dataBits);
    setParity(default_parity);
    setStopBits(default_stopBits);
    setFlowControl(default_flowControl);

    state = true;
    return true;
}
//--------------------------------------------------------------------------------
bool FakeSerial::close(void)
{
    emit signalStatus("FakeSerial close", QDateTime::currentDateTime());
    state = false;
    return true;
}
//--------------------------------------------------------------------------------
QByteArray FakeSerial::readAll(void)
{
    emit debug("readAll()");
    if(emulator_v786_2)
    {
        output_data.clear();
        output_data.append(emulator_v786_2->get_answer());
    }
    return output_data;
}
//--------------------------------------------------------------------------------
void FakeSerial::write(const char *data, int len)
{
    emit debug("write(const char *data, int len)");
    input_data.append(data, len);
}
//--------------------------------------------------------------------------------
void FakeSerial::write(const QByteArray &data)
{
    emit debug(QString("write[0x%1]").arg(data.toHex().data()));

    if(emulator_v786_2)
    {
        emulator_v786_2->set_question(data);
        emit readyRead();
    }
}
//--------------------------------------------------------------------------------
void FakeSerial::wait_msec(int timeout_msec)
{
    QTime time;

    time.start();
    while(true)
    {
        QCoreApplication::processEvents();
        if(time.elapsed() > timeout_msec)
            break;
    }
}
//--------------------------------------------------------------------------------
