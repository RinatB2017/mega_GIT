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
#ifndef CONTROLLOG_HPP
#define CONTROLLOG_HPP
//--------------------------------------------------------------------------------
#include <QObject>
#include <QFile>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
class ControlLog : public QObject
{
    Q_OBJECT
public:
    explicit ControlLog(const QString log_name,
                        QObject *parent = nullptr);
    ~ControlLog();

#ifndef CALIBRATION_WIRED
    void set_modification(const QString &modification_string);
#endif
    void set_timeout_calibrator(int data);
    void set_timeout_multimeter(int data);
    void set_serial_number(int data);
    void set_serial_year(int data);
#ifndef CALIBRATION_WIRED
    void set_address(int data);
    void set_channel(int data);
#endif
    void set_date(const QString &date_string);
    void set_time(const QString &time_string);
    void set_step(const QString &data);
    void set_result(const QString &data);
    void set_time_step_ms(int time_step);
    void save(void);

private:
    QFile   *log_file;
    QString log_name;

#ifndef CALIBRATION_WIRED
    QString modification;
#endif
    int timeout_calibrator;
    int timeout_multimeter;
    int serial_number;
    int serial_year;
#ifndef CALIBRATION_WIRED
    int address;
    int channel;
#endif
    QString date;
    QString time;
    QString step;
    QString result;
    int time_step_ms;

};
//--------------------------------------------------------------------------------
#endif // CONTROLLOG_HPP
