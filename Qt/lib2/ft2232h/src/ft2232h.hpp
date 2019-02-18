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
#ifndef FT2232_HPP
#define FT2232_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#include <ftd2xx.h>

#include "mywidget.hpp"
//--------------------------------------------------------------------------------
enum I2C_Freq {
    I2C_100kHz,
    I2C_200kHz,
    I2C_400kHz
};
//--------------------------------------------------------------------------------
class FT2232H : public MyWidget
{
    Q_OBJECT
public:
    FT2232H(I2C_Freq freq, QWidget *parent = nullptr);

    bool prepare(void);

    FT_STATUS open(int deviceNumber, bool silence = false);
    BOOL close(void);

    void clear_data(void);
    void append_data(bool data);
    FT_STATUS write_data(void);

    FT_STATUS createDeviceInfoList(LPDWORD lpdwNumDevs);
    FT_STATUS setUSBParameters(ULONG ulInTransferSize,
                               ULONG ulOutTransferSize);
    FT_STATUS setChars(UCHAR EventChar,
                       UCHAR EventCharEnabled,
                       UCHAR ErrorChar,
                       UCHAR ErrorCharEnabled);
    FT_STATUS setTimeouts(ULONG ReadTimeout,
                          ULONG WriteTimeout);
    FT_STATUS setLatencyTimer(UCHAR ucLatency);
    FT_STATUS setBitMode(UCHAR ucMask,
                         UCHAR ucEnable);

    FT_STATUS read(BYTE *InputBuffer,
                   int cnt,
                   DWORD *dwNumBytesRead);
    FT_STATUS getQueueStatus(DWORD *dwNumInputBuffer);
    FT_STATUS resetDevice(void);

    void print_error(const QString &function,
                     FT_STATUS status);

private:
    FT_STATUS ftStatus;		// Status defined in D2XX to indicate operation result
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

    void list_devices(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif // FT2232_HPP
