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

#include <ftd2xx.h>
#include <ftdi.h>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class HD44780 : public MyWidget
{
    Q_OBJECT
public:
    HD44780(QWidget *parent = nullptr);
    ~HD44780();

    bool open(void);
    void close(void);

    void test(void);

private:
    FT_STATUS ftStatus;			//Status defined in D2XX to indicate operation result
    FT_HANDLE ftHandle;         // Handle of FT2232H device port

    DWORD dwClockDivisor;

    BYTE OutputBuffer[1024];    // Buffer to hold MPSSE commands and data to be sent to FT2232H
    DWORD dwNumBytesToSend;     // Index of output buffer
    DWORD dwNumInputBuffer;

    BYTE InputBuffer[1024];		// Buffer to hold Data bytes to be read from FT2232H
    DWORD dwNumBytesSent;
    DWORD dwNumBytesRead;

    void screen_on(void);
    void screen_off(void);
    void clear(void);
    void move_cursor(void);
    void write(unsigned char data);

    void strobe(void);

    bool VerifyMPSSE(void);
    void ConfigureMPSSE(void);

    void print_error(const QString &function,
                     FT_STATUS status);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // HD44780_HPP
