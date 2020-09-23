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
#include "ft2232h_thread.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
//const BYTE MSB_VEDGE_CLOCK_IN_BIT = 0x22;
//const BYTE MSB_EDGE_CLOCK_OUT_BYTE = 0x11;
//const BYTE MSB_EDGE_CLOCK_IN_BYTE = 0x24;

//const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_IN = 0x24;
//const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT = 0x11;
//const BYTE MSB_DOWN_EDGE_CLOCK_BIT_IN = 0x26;
//const BYTE MSB_UP_EDGE_CLOCK_BYTE_IN = 0x20;
//const BYTE MSB_UP_EDGE_CLOCK_BYTE_OUT = 0x10;
//const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN = 0x22;
//--------------------------------------------------------------------------------
#define DIR_SPI '\x13'  // bit4:CS, bit3:XXX, bit2:MISO, bit1:MOSI, bit0:SCK
//--------------------------------------------------------------------------------
union FT2232H_DATA
{
    unsigned short value;
    struct
    {
        unsigned char hi;
        unsigned char lo;
    } bytes;
};
//--------------------------------------------------------------------------------
FT2232H_thread::FT2232H_thread(QObject *parent) :
    QThread(parent),
    dwClockDivisor(0x004A)    // 400kHz
{
}
//--------------------------------------------------------------------------------
void FT2232H_thread::run(void)
{
    BOOL ok = open(0);
    if(ok == FALSE)
        return;

    if(SPI_Initial(ftdiHandle) != TRUE)
    {
        emit error("SPI_Initial return FALSE");
        return;
    }

    int cnt_error = 0;
    int x = 0;
    while(1)
    {
        dwNumBytesToSend = 0;
        OutputBuffer[dwNumBytesToSend++] = 0x83;
        OutputBuffer[dwNumBytesToSend++] = 0x81;
        OutputBuffer[dwNumBytesToSend++] = 0x87;
        ftStatus = FT_Write(ftdiHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the BAD commands
        if(ftStatus != FT_OK)
        {
            print_error("run: FT_Write", ftStatus);
            cnt_error++;
        }
        ftStatus = FT_GetQueueStatus(ftdiHandle, &dwNumInputBuffer);  // Get the number of bytes in the device input buffer
        if(ftStatus != FT_OK)
        {
            print_error("run: FT_GetQueueStatus", ftStatus);
            cnt_error++;
        }
        ftStatus = FT_Read(ftdiHandle, InputBuffer, dwNumInputBuffer, &dwNumBytesRead);   // Read out the data from input buffer
        if(ftStatus != FT_OK)
        {
            print_error("run: FT_Read", ftStatus);
            cnt_error++;
        }
        if(dwNumBytesRead == 2)
        {
            FT2232H_DATA data;
            data.bytes.hi = InputBuffer[0];
            data.bytes.lo = InputBuffer[1];

            //emit info(QString("Read 0x%1").arg(data.value));
            emit send_value(x, data.value);
            x++;
        }
        Sleeper::msleep(10);

        if(cnt_error>10) break;
    }

    close();
}
//--------------------------------------------------------------------------------
//this routine is used to enable SPI device
void FT2232H_thread::SPI_CSEnable(void)
{
    for(int loop=0; loop<5; loop++) // one 0x80 command can keep 0.2us, do 5 times to stay in this situation for 1us
    {
        OutputBuffer[dwNumBytesToSend++] = 0x80;  // GPIO command for ADBUS
        OutputBuffer[dwNumBytesToSend++] = 0x10;  // set CS high, MOSI and SCL low
        OutputBuffer[dwNumBytesToSend++] = DIR_SPI;
    }
}
//--------------------------------------------------------------------------------
//this routine is used to disable SPI device
void FT2232H_thread::SPI_CSDisable(void)
{
    for(int loop=0; loop<5; loop++) // one 0x80 command can keep 0.2us, do 5 times to stay in this situation for 1us
    {
        OutputBuffer[dwNumBytesToSend++] = 0x80;  // GPIO command for ADBUS
        OutputBuffer[dwNumBytesToSend++] = 0x00;  // set CS, MOSI and SCL low
        OutputBuffer[dwNumBytesToSend++] = DIR_SPI;
    }
}
//--------------------------------------------------------------------------------
//this routine is used to initial SPI interface
BOOL FT2232H_thread::SPI_Initial(FT_HANDLE ftHandle)
{
    DWORD dwCount;
    ftStatus = FT_ResetDevice(ftHandle); // Reset USB device
    // Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
    ftStatus |= FT_GetQueueStatus(ftHandle, &dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer
    if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
        ftStatus |= FT_Read(ftHandle, InputBuffer, dwNumInputBuffer, &dwNumBytesRead); // Read out the data from FT2232H receive buffer
    ftStatus |= FT_SetUSBParameters(ftHandle, 65535, 65535);    // Set USB request transfer size
    ftStatus |= FT_SetChars(ftHandle, false, 0, false, 0);      // Disable event and error characters
    ftStatus |= FT_SetTimeouts(ftHandle, 3000, 3000);           // Sets the read and write timeouts in 3 sec for the FT2232H
    ftStatus |= FT_SetLatencyTimer(ftHandle, 1);                // Set the latency timer
    ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x00);             // Reset controller
    ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x02);             // Enable MPSSE mode
    if (ftStatus != FT_OK)
    {
        print_error("SPI_Initial", ftStatus);
        return FALSE;
    }
    Sleeper::msleep(50); // Wait 50ms for all the USB stuff to complete and work

    //////////////////////////////////////////////////////////////////
    // Synchronize the MPSSE interface by sending bad command &xAA*
    //////////////////////////////////////////////////////////////////
    dwNumBytesToSend = 0;
    OutputBuffer[dwNumBytesToSend++] = 0xAA; // Add BAD command &xAA*
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the BAD commands
    dwNumBytesToSend = 0; // Clear output buffer
    do{
        ftStatus = FT_GetQueueStatus(ftHandle, &dwNumInputBuffer);  // Get the number of bytes in the device input buffer
    }while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK));        // or Timeout

    bool bCommandEchod = false;
    ftStatus = FT_Read(ftHandle, InputBuffer, dwNumInputBuffer, &dwNumBytesRead); // Read out the data from input buffer
    for (dwCount = 0; dwCount < (dwNumBytesRead - 1); dwCount++)    // Check if Bad command and echo command received
    {
        if ((InputBuffer[dwCount] == BYTE('\xFA')) && (InputBuffer[dwCount+1] == BYTE('\xAA')))
        {
            bCommandEchod = true;
            break;
        }
    }
    if (bCommandEchod == false)
    {
        emit error("fail to synchronize MPSSE with command '0xAA'");
        return FALSE; /* Error, can*t receive echo command , fail to synchronize MPSSE interface; */
    }
    //////////////////////////////////////////////////////////////////
    // Synchronize the MPSSE interface by sending bad command &xAB*
    //////////////////////////////////////////////////////////////////
    dwNumBytesToSend = 0; // Clear output buffer
    OutputBuffer[dwNumBytesToSend++] = 0xAB;  // Send BAD command &xAB*
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send off the BAD commands

    dwNumBytesToSend = 0;   // Clear output buffer
    do{
        ftStatus = FT_GetQueueStatus(ftHandle, &dwNumInputBuffer);  // Get the number of bytes in the device input buffer
    }while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK));        // or Timeout

    bCommandEchod = false;
    ftStatus = FT_Read(ftHandle, InputBuffer, dwNumInputBuffer, &dwNumBytesRead);   // Read out the data from input buffer
    for (dwCount = 0; dwCount < (dwNumBytesRead - 1); dwCount++)                    // Check if Bad command and echo command received
    {
        if ((InputBuffer[dwCount] == BYTE('\xFA')) && (InputBuffer[dwCount+1] == BYTE( '\xAB')))
        {
            bCommandEchod = true;
            break;
        }
    }
    if (bCommandEchod == false)
    {
        emit error("fail to synchronize MPSSE with command '0xAB'");
        return FALSE; /*Error, can't receive echo command , fail to synchronize MPSSE interface;*/
    }
    ////////////////////////////////////////////////////////////////////
    //Configure the MPSSE for SPI communication with EEPROM
    //////////////////////////////////////////////////////////////////
    OutputBuffer[dwNumBytesToSend++] = 0x8A;  // Ensure disable clock divide by 5 for 60Mhz master clock
    OutputBuffer[dwNumBytesToSend++] = 0x97;  // Ensure turn off adaptive clocking
    OutputBuffer[dwNumBytesToSend++] = 0x8D;  // disable 3 phase data clock
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send out the commands
    dwNumBytesToSend = 0;   // Clear output buffer
    OutputBuffer[dwNumBytesToSend++] = 0x80;  // Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = 0x00;  // Set SDA, SCL high, WP disabled by SK, DO at bit &*, GPIOL0 at bit &*
    OutputBuffer[dwNumBytesToSend++] = DIR_SPI;  // Set SK,DO,GPIOL0 pins as output with bit **, other pins as input with bit &*
    // The SK clock frequency can be worked out by below algorithm with divide by 5 set as off
    // SK frequency = 60MHz /((1 + [(1 +0xValueH*256) OR 0xValueL])*2)
    OutputBuffer[dwNumBytesToSend++] = 0x86;  // Command to set clock divisor
    OutputBuffer[dwNumBytesToSend++] = BYTE(dwClockDivisor & 0xFF);   // Set 0xValueL of clock divisor
    OutputBuffer[dwNumBytesToSend++] = BYTE(dwClockDivisor >> 8);       // Set 0xValueH of clock divisor
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send out the commands
    if(ftStatus != FT_OK)
    {
        print_error("SPI_Initial(1): FT_Write", ftStatus);
    }
    Sleeper::msleep(20);    // Delay for a while

    //Turn off loop back in case
    dwNumBytesToSend = 0;                       // Clear output buffer
    OutputBuffer[dwNumBytesToSend++] = 0x85;  // Command to turn off loop back of TDI/TDO connection
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); // Send out the commands
    if(ftStatus != FT_OK)
    {
        print_error("SPI_Initial(2): FT_Write", ftStatus);
    }

    dwNumBytesToSend = 0;   // Clear output buffer
    Sleeper::msleep(30);    // Delay for a while
    if(ftStatus != FT_OK)
    {
        print_error("SPI_Initial(3): FT_Write", ftStatus);
    }
    emit info("SPI initial successful");
    return TRUE;
}
//--------------------------------------------------------------------------------
BOOL FT2232H_thread::open(int deviceNumber)
{
    DWORD numDevs;
    FT_DEVICE_LIST_INFO_NODE *devInfo;

    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    if(ftStatus != FT_OK)
    {
        print_error("open", ftStatus);
        return FALSE;
    }

    if(numDevs <= 0)
    {
        emit error(QString("open: numDevs = %1").arg(numDevs));
        return FALSE;
    }
    else
    {
        emit info(QString("Number of devices is %1").arg(numDevs));
    }

    // allocate storage for list based on numDevs
    devInfo = reinterpret_cast<FT_DEVICE_LIST_INFO_NODE*>(malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs));
    // get the device information list
    ftStatus = FT_GetDeviceInfoList(devInfo,&numDevs);
    if (ftStatus != FT_OK)
    {
        print_error("open: FT_GetDeviceInfoList", ftStatus);
        return FALSE;
    }

    emit info(QString("Open device: %1").arg(deviceNumber));
    ftStatus = FT_Open(deviceNumber, &ftdiHandle);
    if(ftStatus != FT_OK)
    {
        print_error("Can't open FT2232H device: open", ftStatus);
        return FALSE;
    }

    emit info("Successfully open FT2232H device!");
    return TRUE;
}
//--------------------------------------------------------------------------------
BOOL FT2232H_thread::close(void)
{
    FT_STATUS ftStatus = FT_OK;
    ftStatus = FT_Close(ftdiHandle);
    if(ftStatus != FT_OK)
    {
        print_error("close", ftStatus);
        return FALSE;
    }
    ftdiHandle = nullptr;
    return TRUE;
}
//--------------------------------------------------------------------------------
void FT2232H_thread::print_error(const QString &function,
                                 FT_STATUS status)
{
    switch(status)
    {
    case FT_OK:                     break;
    case FT_INVALID_HANDLE:         emit error(QString("%1 return FT_INVALID_HANDLE").arg(function)); break;
    case FT_DEVICE_NOT_FOUND:       emit error(QString("%1 return FT_DEVICE_NOT_FOUND").arg(function)); break;
    case FT_DEVICE_NOT_OPENED:      emit error(QString("%1 return FT_DEVICE_NOT_OPENED").arg(function)); break;
    case FT_IO_ERROR:               emit error(QString("%1 return FT_IO_ERROR").arg(function)); break;
    case FT_INSUFFICIENT_RESOURCES: emit error(QString("%1 return FT_INSUFFICIENT_RESOURCES").arg(function)); break;
    case FT_INVALID_PARAMETER:      emit error(QString("%1 return FT_INVALID_PARAMETER").arg(function)); break;
    case FT_INVALID_BAUD_RATE:      emit error(QString("%1 return FT_INVALID_BAUD_RATE").arg(function)); break;
    case FT_DEVICE_NOT_OPENED_FOR_ERASE: emit error(QString("%1 return FT_DEVICE_NOT_OPENED_FOR_ERASE").arg(function)); break;
    case FT_DEVICE_NOT_OPENED_FOR_WRITE: emit error(QString("%1 return FT_DEVICE_NOT_OPENED_FOR_WRITE").arg(function)); break;
    case FT_FAILED_TO_WRITE_DEVICE: emit error(QString("%1 return FT_FAILED_TO_WRITE_DEVICE").arg(function)); break;
    case FT_EEPROM_READ_FAILED:     emit error(QString("%1 return FT_EEPROM_READ_FAILED").arg(function)); break;
    case FT_EEPROM_WRITE_FAILED:    emit error(QString("%1 return FT_EEPROM_WRITE_FAILED").arg(function)); break;
    case FT_EEPROM_ERASE_FAILED:    emit error(QString("%1 return FT_EEPROM_ERASE_FAILED").arg(function)); break;
    case FT_EEPROM_NOT_PRESENT:     emit error(QString("%1 return FT_EEPROM_NOT_PRESENT").arg(function)); break;
    case FT_EEPROM_NOT_PROGRAMMED:  emit error(QString("%1 return FT_EEPROM_NOT_PROGRAMMED").arg(function)); break;
    case FT_INVALID_ARGS:           emit error(QString("%1 return FT_INVALID_ARGS").arg(function)); break;
    case FT_NOT_SUPPORTED:          emit error(QString("%1 return FT_NOT_SUPPORTED").arg(function)); break;
    case FT_OTHER_ERROR:            emit error(QString("%1 return FT_OTHER_ERROR").arg(function)); break;
    default: emit error(QString("unknown error %1").arg(status)); break;
    }
}
//--------------------------------------------------------------------------------
