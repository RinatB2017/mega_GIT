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
#ifndef BATTERYPROGRESSBAR_HPP
#define BATTERYPROGRESSBAR_HPP
//--------------------------------------------------------------------------------
#include <QtGlobal>
#ifdef Q_OS_WIN
#   include <stdint.h>
#endif
//--------------------------------------------------------------------------------
#if QT_VERSION < QT_VERSION_CHECK(5,4,0)
#   define nullptr NULL
#endif
//--------------------------------------------------------------------------------
#include <QProgressBar>
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
class BatteryProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit BatteryProgressBar(QWidget *parent = nullptr);

signals:

public slots:
    void setVoltage(uint16_t voltage_value_mV);
    void setGrayPalette(void);
    void reset(void);

private:
    int min_value = 0;
    int max_value = MAX_VALUE_MV;

    bool flag_only_down = true;
    int old_value = 0;
};
//--------------------------------------------------------------------------------
#endif // BATTERYPROGRESSBAR_HPP
