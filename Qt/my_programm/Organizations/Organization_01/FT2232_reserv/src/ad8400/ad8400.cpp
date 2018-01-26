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
#include <QCoreApplication>
//--------------------------------------------------------------------------------
#include "ad8400.hpp"
#include "sleeper.h"
#include "i2c.hpp"
//--------------------------------------------------------------------------------
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
AD8400::AD8400(I2C_Freq freq,
               QWidget *parent) :
    MyWidget(parent)
{
    ftHandle = 0;
    dwNumBytesToSend = 0;

    //Value of clock divisor, SCL Frequency = 60/((1+0x004A)*2) (MHz) = 400khz
    // x = (30 / f) - 1
    switch(freq)
    {
    case I2C_400kHz:
        dwClockDivisor = 0x004A;    // 400kHz
        emit info("частота I2C = 400кГц");
        break;
    case I2C_200kHz:
        dwClockDivisor = 0x0095;    // 200khz
        emit info("частота I2C = 200кГц");
        break;
    case I2C_100kHz:
        dwClockDivisor = 0x012B;    // 100kHz
        emit info("частота I2C = 100кГц");
        break;
    }
}
//--------------------------------------------------------------------------------
bool AD8400::VerifyMPSSE(void)
{
    //////////////////////////////////////////////////////////////////
    // Synchronize the MPSSE interface by sending bad command '0xAA'
    //////////////////////////////////////////////////////////////////
    DWORD dwCount;
    OutputBuffer[dwNumBytesToSend++] = '\xAA';		//Add BAD command '0xAA'
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the BAD commands
    if(ftStatus != FT_OK)
    {
        print_error("VerifyMPSSE: FT_Write(1)", ftStatus);
    }
    dwNumBytesToSend = 0;			//Clear output buffer
    int err_cnt = 0;
    do{
        ftStatus = FT_GetQueueStatus(ftHandle, &dwNumInputBuffer);      // Get the number of bytes in the device input buffer
        Sleeper::msleep(1);
        err_cnt++;
    }while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK) && (err_cnt<1000));            // or Timeout

    bool bCommandEchod = false;
    ftStatus = FT_Read(ftHandle, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead);  //Read out the data from input buffer
    if(ftStatus != FT_OK)
    {
        emit error(QString("VerifyMPSSE: FT_Read(1) return %1").arg(ftStatus));
    }
    if(dwNumBytesRead < 1)
    {
        emit error(QString("VerifyMPSSE: dwNumBytesRead = %1").arg(dwNumBytesRead));
        return false;
    }
    for (dwCount = 0; dwCount < dwNumBytesRead - 1; dwCount++)                      //Check if Bad command and echo command received
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
        return false; /*Error, can't receive echo command , fail to synchronize MPSSE interface;*/
    }

    //////////////////////////////////////////////////////////////////
    // Synchronize the MPSSE interface by sending bad command '0xAB'
    //////////////////////////////////////////////////////////////////
    //dwNumBytesToSend = 0;                     //Clear output buffer
    OutputBuffer[dwNumBytesToSend++] = '\xAB';	//Send BAD command '0xAB'
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the BAD commands
    if(ftStatus != FT_OK)
    {
        print_error("VerifyMPSSE: FT_Write(2)", ftStatus);
    }
    dwNumBytesToSend = 0;                       //Clear output buffer

    err_cnt = 0;
    do{
        ftStatus = FT_GetQueueStatus(ftHandle, &dwNumInputBuffer);                  //Get the number of bytes in the device input buffer
        Sleeper::msleep(1);
        err_cnt++;
    }while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK) && (err_cnt<1000));      //or Timeout
    bCommandEchod = false;
    ftStatus = FT_Read(ftHandle, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead);  //Read out the data from input buffer
    for(dwCount = 0; dwCount<dwNumBytesRead-1; dwCount++)                           //Check if Bad command and echo command received
    {
        if ((InputBuffer[dwCount] == BYTE('\xFA')) && (InputBuffer[dwCount+1] == BYTE('\xAB')))
        {
            bCommandEchod = true;
            break;
        }
    }
    if (bCommandEchod == false)
    {
        emit error("fail to synchronize MPSSE with command '0xAB'");
        return false;
        /*Error, can't receive echo command , fail to synchronize MPSSE interface;*/
    }

    //emit info("MPSSE synchronized with BAD command");
    return true;
}
//--------------------------------------------------------------------------------
void AD8400::ConfigureMPSSE(void)
{
    ////////////////////////////////////////////////////////////////////
    // Configure the MPSSE for I2C communication with 24LC02B
    //////////////////////////////////////////////////////////////////
    OutputBuffer[dwNumBytesToSend++] = '\x8A';  //Ensure disable clock divide by 5 for 60Mhz master clock
    OutputBuffer[dwNumBytesToSend++] = '\x97';	//Ensure turn off adaptive clocking
    OutputBuffer[dwNumBytesToSend++] = '\x8D'; 	//Enable 3 phase data clock, used by I2C to allow data on both clock edges
    // Send off the commands
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("ConfigureMPSSE: FT_Write(1)", ftStatus);
    }
    dwNumBytesToSend = 0;                       //Clear output buffer
    OutputBuffer[dwNumBytesToSend++] = '\x80';	// Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = '\x03'; 	//Set SDA, SCL high, WP disabled by SK, DO at bit '1', GPIOL0 at bit '0'
    OutputBuffer[dwNumBytesToSend++] = '\x13';  //Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    // The SK clock frequency can be worked out by below algorithm with divide by 5 set as off
    // SK frequency  = 60MHz /((1 +  [(1 +0xValueH*256) OR 0xValueL])*2)
    OutputBuffer[dwNumBytesToSend++] = '\x86';                            //Command to set clock divisor
    OutputBuffer[dwNumBytesToSend++] = dwClockDivisor & '\xFF';           //Set 0xValueL of clock divisor
    OutputBuffer[dwNumBytesToSend++] = (dwClockDivisor >> 8) & '\xFF';	  //Set 0xValueH of clock divisor
    // Send off the commands
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("ConfigureMPSSE: FT_Write(2)", ftStatus);
    }
    dwNumBytesToSend = 0;		//Clear output buffer
    Sleeper::msleep(20);		//Delay for a while

    //Turn off loop back in case
    OutputBuffer[dwNumBytesToSend++] = '\x85';		//Command to turn off loop back of TDI/TDO connection
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the commands
    if(ftStatus != FT_OK)
    {
        emit error(QString("ConfigureMPSSE: FT_Write(3) return %1").arg(ftStatus));
    }
    dwNumBytesToSend = 0;		//Clear output buffer
    Sleeper::msleep(30);		//Delay for a while
}
//--------------------------------------------------------------------------------
BOOL AD8400::open(int deviceNumber)
{
    DWORD devIndex = 0;
    char Buf[64];
    DWORD numDevs;

    if(ftHandle)
    {
        emit info("FT2232H is open!");
        return true;
    }

    ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
    if (ftStatus != FT_OK)
    {
        print_error("Can't open FT2232H device! FT_ListDevices", ftStatus);
        return false;
    }
    ftStatus = FT_ListDevices((PVOID)devIndex ,&Buf, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
    if (ftStatus != FT_OK)
    {
        print_error("Can't open FT2232H device! FT_ListDevices(2)", ftStatus);
        return false;
    }

    ftStatus = FT_Open(deviceNumber, &ftHandle);
    if (ftStatus != FT_OK)
    {
        print_error("Can't open FT2232H device! FT_Open", ftStatus);
        return false;
    }
    else
    {   // Port opened successfully
        emit info("Successfully open FT2232H device!");

        ftStatus |= FT_ResetDevice(ftHandle); 	// Reset USB device
        // Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
        ftStatus |= FT_GetQueueStatus(ftHandle, &dwNumInputBuffer);	 // Get the number of bytes in the FT2232H receive buffer
        if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
            FT_Read(ftHandle, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead);  	// Read out the data from FT2232H receive buffer
        ftStatus |= FT_SetUSBParameters(ftHandle, 65536, 65535);    // Set USB request transfer size
        ftStatus |= FT_SetChars(ftHandle, false, 0, false, 0);      // Disable event and error characters
        ftStatus |= FT_SetTimeouts(ftHandle, 0, 5000);              // Sets the read and write timeouts in milliseconds for the FT2232H
        ftStatus |= FT_SetLatencyTimer(ftHandle, 16);               // Set the latency timer
        ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x00);             // Reset controller
        ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x02);             // Enable MPSSE mode

        if (ftStatus != FT_OK)
        {
            print_error("fail on initialize FT2232H device 1!", ftStatus);
            return false;
        }
        Sleeper::msleep(50);	// Wait for all the USB stuff to complete and work

        if(!VerifyMPSSE())
            return false;

        ConfigureMPSSE();

        dwNumBytesToSend = 0;   // Clear output buffer
    }
    return true;
}
//--------------------------------------------------------------------------------
void AD8400::close(void)
{
    ftStatus = FT_Close(ftHandle);
    if(ftStatus == FT_OK)
        ftHandle = 0;
}
//--------------------------------------------------------------------------------
BOOL AD8400::write(BYTE data)
{
    emit debug(QString("send %1").arg(data));
    FT_STATUS ftStatus = FT_OK;
    dwNumBytesToSend = 0;
    OutputBuffer[dwNumBytesToSend++] = data;
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("write", ftStatus);
        return FALSE;
    }
    return TRUE;
}
//--------------------------------------------------------------------------------
void AD8400::test(void)
{
    emit info("test AD8400");
    if(open(1))
    {
        for(int n=0; n<255; n++)
        {
            QCoreApplication::processEvents();
            write(n);
            Sleeper::msleep(10);
        }
        close();
    }
    else
    {
        emit error("test failed!");
    }
    emit info("test end");
}
//--------------------------------------------------------------------------------
void AD8400::print_error(const QString &function,
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
void AD8400::updateText(void)
{

}
//--------------------------------------------------------------------------------
