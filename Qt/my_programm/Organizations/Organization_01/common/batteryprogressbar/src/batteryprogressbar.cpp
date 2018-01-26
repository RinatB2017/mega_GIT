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
// Зеленый  		66CC33		>=2.3В
// Желтый			FFFF66		>=2.1В
// Красный			FF6633		>=1.8B
// max 2.5В
#define COLOR_GREEN     QColor(0x66, 0xCC, 0x33)
#define COLOR_YELLOW    QColor(0xFF, 0xFF, 0x66)
#define COLOR_RED       QColor(0xFF, 0x33, 0x00)
#define COLOR_GRAY      QColor(0x80, 0x80, 0x80)

#define VOLTAGE_RED     2100
#define VOLTAGE_YELLOW  2300

#define MIN_VALUE_MV   1800
#define MAX_VALUE_MV   2700
//--------------------------------------------------------------------------------
BatteryProgressBar::BatteryProgressBar(int min_value,
                                       int max_value,
                                       bool only_down,
                                       QWidget *parent) :
    QProgressBar(parent),
    min_value(min_value),
    max_value(max_value),
    flag_only_down(only_down)
{
    reset();

    setMinimum(min_value);
    setMaximum(max_value);

    setTextVisible(false);
    setFixedSize(100, 16);
}
//--------------------------------------------------------------------------------
BatteryProgressBar::BatteryProgressBar(bool only_down,
                                       QWidget *parent) :
    QProgressBar(parent),
    flag_only_down(only_down)
{
    flag_only_down ? old_value = MAX_VALUE_MV : old_value = 0;

    min_value = MIN_VALUE_MV;
    max_value = MAX_VALUE_MV;

    setMinimum(min_value);
    setMaximum(max_value);

    setTextVisible(false);
    setFixedSize(100, 16);
}
//--------------------------------------------------------------------------------
void BatteryProgressBar::setValue(int value)
{
    QProgressBar::setValue(value);
}
//--------------------------------------------------------------------------------
void BatteryProgressBar::setVoltage(unsigned short voltage_value_mV)
{
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

    //qDebug() << voltage_value_mV;
    if(voltage_value_mV < MIN_VALUE_MV)
    {
        QProgressBar::setValue(0);
        return;
    }
    if(voltage_value_mV < VOLTAGE_RED)
    {
        p.setColor(QPalette::Highlight, COLOR_RED);
        setPalette(p);
        QProgressBar::setValue(voltage_value_mV);
        return;
    }
    if(voltage_value_mV < VOLTAGE_YELLOW)
    {
        p.setColor(QPalette::Highlight, COLOR_YELLOW);
        setPalette(p);
        QProgressBar::setValue(voltage_value_mV);
        return;
    }
    else
    {
        p.setColor(QPalette::Highlight, COLOR_GREEN);
        setPalette(p);
        QProgressBar::setValue(voltage_value_mV);
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
