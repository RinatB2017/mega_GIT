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
#include "controllog.hpp"
//--------------------------------------------------------------------------------
ControlLog::ControlLog(const QString log_name,
                       QObject *parent) :
    QObject(parent),
    log_name(log_name)
{
#ifndef CALIBRATION_WIRED
    modification    = "no modification";
#endif
    serial_number   = -1;
    serial_year     = -1;
#ifndef CALIBRATION_WIRED
    address         = -1;
    channel         = -1;
#endif
    date            = "no date";
    time            = "no time";
    step            = "no step";
    result          = "no result";
    time_step_ms    = -1;

    log_file = new QFile(log_name);
}
//--------------------------------------------------------------------------------
ControlLog::~ControlLog()
{
    if(log_file->isOpen()) log_file->close();
}
//--------------------------------------------------------------------------------
#ifndef CALIBRATION_WIRED
void ControlLog::set_modification(const QString &modification_string)
{
    modification = modification_string;
}
#endif
//--------------------------------------------------------------------------------
void ControlLog::set_timeout_calibrator(int data)
{
    timeout_calibrator = data;
}
//--------------------------------------------------------------------------------
void ControlLog::set_timeout_multimeter(int data)
{
    timeout_multimeter = data;
}
//--------------------------------------------------------------------------------
void ControlLog::set_serial_number(int data)
{
    serial_number = data;
}
//--------------------------------------------------------------------------------
void ControlLog::set_serial_year(int data)
{
    serial_year = data;
}
//--------------------------------------------------------------------------------
#ifndef CALIBRATION_WIRED
void ControlLog::set_address(int data)
{
    address = data;
}
#endif
//--------------------------------------------------------------------------------
#ifndef CALIBRATION_WIRED
void ControlLog::set_channel(int data)
{
    channel = data;
}
#endif
//--------------------------------------------------------------------------------
void ControlLog::set_date(const QString &date_string)
{
    date = date_string;
}
//--------------------------------------------------------------------------------
void ControlLog::set_time(const QString &time_string)
{
    time = time_string;
}
//--------------------------------------------------------------------------------
void ControlLog::set_step(const QString &data)
{
    step = data;
}
//--------------------------------------------------------------------------------
void ControlLog::set_result(const QString &data)
{
    result = data;
}
//--------------------------------------------------------------------------------
void ControlLog::set_time_step_ms(int time_step)
{
    time_step_ms = time_step;
}
//--------------------------------------------------------------------------------
void ControlLog::save(void)
{
    if(!log_file->open(QIODevice::Append | QIODevice::Text))
    {
#ifdef QT_DEBUG
        qDebug() << "file" << log_name << "not open!";
#endif
        return;
    }

#ifdef CALIBRATION_WIRED
    QString temp = QString("%1|%2|%3|%4|%5|%6|%7|%8|%9\n")
            .arg(timeout_calibrator)
            .arg(timeout_multimeter)
            .arg(serial_number)
            .arg(serial_year)
            .arg(date)
            .arg(time)
            .arg(step)
            .arg(result)
            .arg(time_step_ms);
#else
    QString temp = QString("%1|%2|%3|%4|%5|%6|%7|%8|%9|%10|%11|%12\n")
            .arg(modification)
            .arg(timeout_calibrator)
            .arg(timeout_multimeter)
            .arg(serial_number)
            .arg(serial_year)
            .arg(address)
            .arg(channel)
            .arg(date)
            .arg(time)
            .arg(step)
            .arg(result)
            .arg(time_step_ms);
#endif

    QByteArray ba;
    ba.append(temp);
    log_file->write(ba);
    log_file->close();
}
//--------------------------------------------------------------------------------
