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
#include "i2c.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
class AD8400 : public MyWidget
{
    Q_OBJECT
public:
    AD8400(I2C_Freq freq, QWidget *parent = 0);

    void test(void);

    BOOL open(int deviceNumber);
    BOOL write(BYTE data);
    void close(void);

private:
    FT_STATUS ftStatus;			// Status defined in D2XX to indicate operation result
    FT_HANDLE ftHandle;         // Handle of FT2232H device port

    DWORD dwClockDivisor;

    BYTE OutputBuffer[1024];    // Buffer to hold MPSSE commands and data to be sent to FT2232H
    DWORD dwNumBytesToSend;     // Index of output buffer

    BYTE InputBuffer[1024];		// Buffer to hold Data bytes to be read from FT2232H
    DWORD dwNumInputBuffer;
    DWORD dwNumBytesSent;
    DWORD dwNumBytesRead;

    bool VerifyMPSSE(void);
    void ConfigureMPSSE(void);
    void print_error(const QString &function,
                     FT_STATUS status);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // AD8400_HPP
