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
#ifndef DS18B20_HPP
#define DS18B20_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class DS18B20 : public MyWidget
{
    Q_OBJECT
public:
    DS18B20(I2C_Freq freq, QWidget *parent = 0);
    ~DS18B20();

    void test(void);

private:
    FT2232H *ft2232h;

    float convert_9bit(unsigned short  temp_ds18b20);
    float convert_10bit(unsigned short temp_ds18b20);
    float convert_11bit(unsigned short temp_ds18b20);
    float convert_12bit(unsigned short temp_ds18b20);

    void prepare_data(void);
    void prepare_data2(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // DS18B20_HPP
