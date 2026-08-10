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
#include <QDebug>
//--------------------------------------------------------------------------------
#include "multimeter_v786_2_struct.hpp"
#include "emulator_v786_2.hpp"
//--------------------------------------------------------------------------------
Emulator_V786_2::Emulator_V786_2(unsigned char address,
                                 unsigned short serno,
                                 unsigned short seryear,
                                 const QString display,
                                 unsigned short voltage,
                                 unsigned short current,
                                 QObject *parent) :
    QObject(parent),
    address(address),
    serno(serno),
    seryear(seryear),
    voltage(voltage),
    current(current)
{
    this->display.clear();
    this->display.append(display);

    init();
}
//--------------------------------------------------------------------------------
void Emulator_V786_2::connect_log(void)
{
    if(parent())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parent(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parent(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parent(), SIGNAL(error(QString)));
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
void Emulator_V786_2::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void Emulator_V786_2::init(void)
{
    connect_log();

    answer.clear();
}
//--------------------------------------------------------------------------------
void Emulator_V786_2::set_question(QByteArray question)
{
    if((unsigned int)question.length() < sizeof(B7_86_STATE))
    {
        emit error(QString(tr("set_question: длина принимаемого пакета слишком мала %1 < %2"))
                   .arg(question.length())
                   .arg(sizeof(B7_86_STATE)));
        return;
    }

    //emit error(question.toHex().data());
    B7_86_STATE_ANSWER *state = (B7_86_STATE_ANSWER *)question.data();
    emit info(QString("address %1").arg(state->header.address));
    emit info(QString("cmd %1").arg(state->header.cmd));

    unsigned char cmd = state->header.cmd;
    answer.clear();
    char *temp;
    switch(cmd)
    {
    case CMD_GET_STATE:
        B7_86_STATE_ANSWER fake_get_state;
        fake_get_state.header.address=address;
        fake_get_state.header.cmd=cmd;
        fake_get_state.header.count_data=8;
        fake_get_state.serial_year.SerialNo=serno;
        fake_get_state.serial_year.SerialYear=seryear;
        for(unsigned int n=0; n<sizeof(fake_get_state.DisplaySymbols); n++)
            fake_get_state.DisplaySymbols[n]=display.at(n).toLatin1();
        fake_get_state.Range_Mode=0;
        fake_get_state.Math_Hold=0;
        fake_get_state.BattVoltage=voltage;
        fake_get_state.crc16=crc16((unsigned char *)&fake_get_state, sizeof(B7_86_STATE_ANSWER) - 2);

        temp = (char *)&fake_get_state;
        for(unsigned int n=0; n<sizeof(B7_86_STATE_ANSWER); n++)
        {
            answer.append((char)temp[n]);
        }
        break;

    case CMD_CALIBRATION_DC_VOLTAGE:
    case CMD_CALIBRATION_AC_VOLTAGE:
    case CMD_CALIBRATION_DC_CURRENT:
    case CMD_CALIBRATION_AC_CURRENT:
    case CMD_CALIBRATION_R:
    case CMD_ENABLING_CHARGE:
    case CMD_DISABLING_CHARGE:
    case CMD_WRITE_TO_FLASH:
    case CMD_REMOTE_CONTROL_ON:
    case CMD_REMOTE_CONTROL_OFF:
    case CMD_POWER_OFF:
    case CMD_RESET:
        B7_86_EMPTY fake_remote_state;
        fake_remote_state.header.address=address;
        fake_remote_state.header.cmd=cmd;
        fake_remote_state.header.count_data=0;
        fake_remote_state.serial_year.SerialNo=serno;
        fake_remote_state.serial_year.SerialYear=seryear;
        fake_remote_state.crc16=crc16((unsigned char *)&fake_remote_state, sizeof(B7_86_EMPTY) - 2);

        temp = (char *)&fake_remote_state;
        for(unsigned int n=0; n<sizeof(B7_86_EMPTY); n++)
        {
            answer.append((char)temp[n]);
        }
        break;

    case CMD_CONTINUITY:
    case CMD_MEASURING_DC_VOLTAGE:
    case CMD_MEASURING_AC_VOLTAGE:
    case CMD_MEASURING_DC_CURRENT:
    case CMD_MEASURING_AC_CURRENT:
    case CMD_MEASURING_R:
    case CMD_MEASURING_C:
    case CMD_MEASURING_L:
    case CMD_MEASURING_F:
    case CMD_MEASURING_ACTIVE_POWER:
    case CMD_MEASURING_REACTIVE_POWER:
    case CMD_MEASURING_FULL_POWER:
    case CMD_MEASURING_POWER_FACTOR:
        B7_86_MEASURING fake_measuring_state;
        fake_measuring_state.header.address=address;
        fake_measuring_state.header.cmd=cmd;
        fake_measuring_state.header.count_data=0;
        fake_measuring_state.serial_year.SerialNo=serno;
        fake_measuring_state.serial_year.SerialYear=seryear;
        for(unsigned int n=0; n<sizeof(fake_measuring_state.DisplaySymbols); n++)
            fake_measuring_state.DisplaySymbols[n]=display.at(n).toLatin1();
        fake_measuring_state.options=0;
        fake_measuring_state.crc16=crc16((unsigned char *)&fake_measuring_state, sizeof(B7_86_MEASURING) - 2);

        temp = (char *)&fake_measuring_state;
        for(unsigned int n=0; n<sizeof(B7_86_MEASURING); n++)
        {
            answer.append((char)temp[n]);
        }
        break;

    case CMD_REQUEST_SETTINGS_CHARGE:
        B7_86_BATTERY fake_battery_state;
        fake_battery_state.header.address=address;
        fake_battery_state.header.cmd=cmd;
        fake_battery_state.header.count_data=8;
        fake_battery_state.serial_year.SerialNo=serno;
        fake_battery_state.serial_year.SerialYear=seryear;
        fake_battery_state.voltage=voltage;
        fake_battery_state.current=current;
        fake_battery_state.crc16=crc16((unsigned char *)&fake_battery_state, sizeof(B7_86_BATTERY) - 2);

        temp = (char *)&fake_battery_state;
        for(unsigned int n=0; n<sizeof(B7_86_BATTERY); n++)
        {
            answer.append((char)temp[n]);
        }
        break;

    default:
        emit error(QString(tr("Неизвестная команда 0x%1")).arg(cmd, 0, 16));
        break;
    }
}
//--------------------------------------------------------------------------------
QByteArray Emulator_V786_2::get_answer(void)
{
    return answer;
}
//--------------------------------------------------------------------------------
unsigned short Emulator_V786_2::crc16(unsigned char *block, unsigned char len)
{
    unsigned short a, crc = 0xFFFF;
    while(len--)
    {
        a = *block++ << 8;
        //crc ^= *block++ << 8;
        crc ^= a;
        for(int i=0; i<8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}
//--------------------------------------------------------------------------------
