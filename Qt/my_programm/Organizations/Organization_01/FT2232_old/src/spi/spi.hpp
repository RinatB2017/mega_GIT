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
#ifndef SPI_HPP
#define SPI_HPP
//--------------------------------------------------------------------------------
#include <QWidget>

#include <ftd2xx.h>
#include <ftdi.h>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define MemSize 16          // define data quantity you want to send out
//--------------------------------------------------------------------------------
class SPI : public MyWidget
{
    Q_OBJECT
public:
    SPI(QWidget *parent = nullptr);

    BOOL open(void);
    BOOL close(void);
    BOOL write(WORD address, WORD bdata);
    BOOL read(WORD address, WORD* bdata);

    BOOL test_at93c56(void);

private:
    FT_STATUS ftStatus;     // Status defined in D2XX to indicate operation result
    BYTE OutputBuffer[512]; // Buffer to hold MPSSE commands and data to be sent to FT2232H
    BYTE InputBuffer[512];  // Buffer to hold Data bytes to be read from FT2232H
    DWORD dwClockDivisor;   // Value of clock divisor, SCL Frequency = 60/((1+29)*2) (MHz) = 1Mhz
    DWORD dwNumBytesToSend; // Index of output buffer
    DWORD dwNumBytesSent, dwNumBytesRead, dwNumInputBuffer;
    BYTE ByteDataRead;
    WORD MemAddress;
    WORD i;
    WORD DataOutBuffer[MemSize];
    WORD DataInBuffer[MemSize];

    FT_HANDLE ftdiHandle;

    void SPI_CSEnable(void);
    void SPI_CSDisable(void);
    FT_STATUS WriteEECmd(FT_HANDLE ftHandle, BYTE command);
    BOOL SPI_Initial(FT_HANDLE ftHandle);
    BOOL SPI_WriteByte2RandomAddr(FT_HANDLE ftHandle, WORD address, WORD bdata);
    BOOL SPI_ReadByteRandomAddr(FT_HANDLE ftHandle, WORD address, WORD* bdata);

    void print_error(const QString &function, FT_STATUS status);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // SPI_HPP
