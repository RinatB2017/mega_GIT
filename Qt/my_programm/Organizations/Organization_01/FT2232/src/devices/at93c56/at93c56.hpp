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
#ifndef I2C_HPP
#define I2C_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#if 0
typedef unsigned char	BYTE;
typedef int             BOOL;
typedef unsigned long 	ULONG;
#endif
typedef ULONG           FT_STATUS;
//--------------------------------------------------------------------------------
#define MAX_ADDRESS 0x7f
//--------------------------------------------------------------------------------
class FT2232H;
//--------------------------------------------------------------------------------
class AT93C56 : public MyWidget
{
    Q_OBJECT
public:
    AT93C56(I2C_Freq freq, QWidget *parent = nullptr);
    ~AT93C56();

    void HighSpeedSetI2CStart(void);
    void HighSpeedSetI2CStop(void);
    BOOL SendByteAndCheckACK(BYTE dwDataSend);

    BOOL test(void);
    void test_read(void);
    void test_write(void);

    void clear_all(void);

    FT_STATUS open(int deviceNumber);
    FT_STATUS read(unsigned char id,
                   unsigned char address,
                   unsigned char *data);
    FT_STATUS write(unsigned char id,
                    unsigned char address,
                    unsigned char data);
    FT_STATUS write_one_byte(unsigned char address,
                             unsigned char data);
    void close(void);

private:
    FT2232H *ft2232h;
    unsigned char  data;
    unsigned short addr;

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // I2C_HPP
