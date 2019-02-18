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
#ifndef AD8400_HPP
#define AD8400_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "at93c56.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class FT2232H;
//--------------------------------------------------------------------------------
class AD8400 : public MyWidget
{
    Q_OBJECT
    
public:
    AD8400(I2C_Freq freq, QWidget *parent = nullptr);
    ~AD8400();

    void test(void);

    FT_STATUS open(int deviceNumber);
    BOOL write(BYTE data);
    void close(void);

private:
    FT2232H *ft2232h;

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // AD8400_HPP
