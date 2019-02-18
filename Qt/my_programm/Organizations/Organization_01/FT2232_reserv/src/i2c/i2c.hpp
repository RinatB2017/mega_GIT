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

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#define MAX_ADDRESS 0x7f
//--------------------------------------------------------------------------------
enum I2C_Freq {
    I2C_100kHz,
    I2C_200kHz,
    I2C_400kHz
};
//--------------------------------------------------------------------------------
class I2C : public MyWidget
{
    Q_OBJECT
    
public:
    I2C(I2C_Freq freq, QWidget *parent = nullptr);

    void HighSpeedSetI2CStart(void);
    void HighSpeedSetI2CStop(void);
    BOOL SendByteAndCheckACK(BYTE dwDataSend);

    void test(void);
    void test_read(void);
    void test_write(void);

    void clear_all(void);

    bool open(int deviceNumber);
    bool read(unsigned char id,
              unsigned char address,
              unsigned char *data);
    bool write(unsigned char id,
               unsigned char address,
               unsigned char data);
    bool write_one_byte(unsigned char address,
                        unsigned char data);
    void close(void);

private:
    FT_STATUS ftStatus;		// Status defined in D2XX to indicate operation result
    FT_HANDLE ftHandle;         // Handle of FT2232H device port

    DWORD dwClockDivisor;

    BYTE OutputBuffer[1024];    // Buffer to hold MPSSE commands and data to be sent to FT2232H
    DWORD dwNumBytesToSend;     // Index of output buffer

    BYTE InputBuffer[1024];	// Buffer to hold Data bytes to be read from FT2232H
    DWORD dwNumInputBuffer;
    DWORD dwNumBytesSent;
    DWORD dwNumBytesRead;

    unsigned char  data;
    unsigned short addr;

    bool VerifyMPSSE(void);
    void ConfigureMPSSE(void);
    void print_error(const QString &function,
                     FT_STATUS status);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // I2C_HPP
