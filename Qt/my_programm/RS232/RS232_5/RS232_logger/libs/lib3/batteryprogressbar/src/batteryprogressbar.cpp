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
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include "batteryprogressbar.hpp"
//--------------------------------------------------------------------------------
BatteryProgressBar::BatteryProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
    reset();

    setMinimum(min_value);
    setMaximum(max_value);
    setValue(0);

    //setTextVisible(false);
    setFixedSize(100, 16);
}
//--------------------------------------------------------------------------------
void BatteryProgressBar::setVoltage(uint16_t voltage_value_mV)
{
    //qDebug() << "setVoltage:" << voltage_value_mV;

    setToolTip(QString("%1 mV").arg(voltage_value_mV));

    if(voltage_value_mV > MAX_VALUE_MV)
        voltage_value_mV = MAX_VALUE_MV;

    if(flag_only_down)
    {
        if(voltage_value_mV > old_value)
        {
            //qDebug() << voltage_value_mV << old_value;
            return;
        }
    }
    old_value = voltage_value_mV;

    QPalette p = palette();

    //qDebug() << "voltage_value_mV:" << voltage_value_mV;
    if(voltage_value_mV < MIN_VALUE_MV)
    {
        setValue(0);
        return;
    }
    if(voltage_value_mV < VOLTAGE_RED)
    {
        //qDebug() << "RED";
        p.setColor(QPalette::Highlight, COLOR_RED);
        setPalette(p);
        setValue(voltage_value_mV);
        return;
    }
    if(voltage_value_mV < VOLTAGE_YELLOW)
    {
        //qDebug() << "YELLOW";
        p.setColor(QPalette::Highlight, COLOR_YELLOW);
        setPalette(p);
        setValue(voltage_value_mV);
        return;
    }
    else
    {
        //qDebug() << "GREEN";
        p.setColor(QPalette::Highlight, COLOR_GREEN);
        setPalette(p);
        setValue(voltage_value_mV);
        return;
    }
}
//--------------------------------------------------------------------------------
void BatteryProgressBar::setGrayPalette(void)
{
    QPalette p = palette();
    p.setColor(QPalette::Highlight, COLOR_GRAY);
    setPalette(p);
}
//--------------------------------------------------------------------------------
void BatteryProgressBar::reset(void)
{
    flag_only_down ? old_value = MAX_VALUE_MV : old_value = MIN_VALUE_MV;
}
//--------------------------------------------------------------------------------
