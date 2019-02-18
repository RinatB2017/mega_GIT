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
#ifndef BITBANG_HPP
#define BITBANG_HPP
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#include <QWidget>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class BitBang : public MyWidget
{
    Q_OBJECT
    
public:
    BitBang(QWidget *parent = nullptr);
    ~BitBang();

    bool open(void);
    void close(void);

public slots:
    void power_on(void);
    void power_off(void);

private:
    FT_STATUS ftStatus;		//Status defined in D2XX to indicate operation result
    FT_HANDLE ftHandle;         // Handle of FT2232H device port

    DWORD dwClockDivisor;

    BYTE OutputBuffer[1024];    // Buffer to hold MPSSE commands and data to be sent to FT2232H
    DWORD dwNumBytesToSend;     // Index of output buffer
    DWORD dwNumInputBuffer;

    BYTE InputBuffer[1024];	// Buffer to hold Data bytes to be read from FT2232H
    DWORD dwNumBytesSent;
    DWORD dwNumBytesRead;

    bool VerifyMPSSE(void);
    void ConfigureMPSSE(void);
    void print_error(const QString &function,
                     FT_STATUS status);

    void updateText(void);

};
//--------------------------------------------------------------------------------
#endif // BITBANG_HPP
