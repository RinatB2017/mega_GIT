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
#include <QProgressBar>
//--------------------------------------------------------------------------------
class BatteryProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit BatteryProgressBar(int min_value,
                                int max_value,
                                bool only_down = false,
                                QWidget *parent = nullptr);
    explicit BatteryProgressBar(bool only_down = false,
                                QWidget *parent = nullptr);

signals:

public slots:
    void setValue(int value);
    void setVoltage(unsigned short voltage_value_mV);
    void setGrayPalette(void);
    void reset(void);

private:
    int min_value;
    int max_value;

    bool flag_only_down;
    int old_value;
};
//--------------------------------------------------------------------------------
#endif // BATTERYPROGRESSBAR_HPP
