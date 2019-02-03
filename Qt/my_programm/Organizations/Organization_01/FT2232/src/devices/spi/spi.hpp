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
#ifndef AT93C56_HPP
#define AT93C56_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include <ftd2xx.h>
//--------------------------------------------------------------------------------
#include "at93c56.hpp"
//--------------------------------------------------------------------------------
#define MemSize 16          // define data quantity you want to send out
//--------------------------------------------------------------------------------
class FT2232H;
//--------------------------------------------------------------------------------
class SPI : public MyWidget
{
    Q_OBJECT
public:
    SPI(I2C_Freq freq, QWidget *parent = 0);

    BOOL open(void);
    BOOL close(void);
    BOOL write(WORD address, WORD bdata);
    BOOL read(WORD address, WORD* bdata);

    BOOL test_at93c56(void);

private:
    FT2232H *ft2232h;
    DWORD dwClockDivisor;

    void SPI_CSEnable(void);
    void SPI_CSDisable(void);
    FT_STATUS WriteEECmd(BYTE command);
    BOOL SPI_Initial(DWORD dwClockDivisor);
    BOOL SPI_WriteByte2RandomAddr(WORD address, WORD bdata);
    BOOL SPI_ReadByteRandomAddr(WORD address, WORD* bdata);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // AT93C56_HPP
