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
#ifndef HD44780_HPP
#define HD44780_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "at93c56.hpp"
//--------------------------------------------------------------------------------
class FT2232H;
//--------------------------------------------------------------------------------
class HD44780 : public MyWidget
{
    Q_OBJECT
public:
    HD44780(I2C_Freq freq, QWidget *parent = 0);
    ~HD44780();

    bool open(int deviceNumber);
    void close(void);

    void test(void);

private:
    FT2232H *ft2232h;

    void screen_on(void);
    void screen_off(void);
    void clear(void);
    void move_cursor(void);
    void write(unsigned char data);

    void strobe(void);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // HD44780_HPP
