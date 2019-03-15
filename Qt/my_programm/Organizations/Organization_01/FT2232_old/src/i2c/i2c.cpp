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
#include <QCoreApplication>
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#include "sleeper.h"
#include "i2c.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
const BYTE MSB_VEDGE_CLOCK_IN_BIT = 0x22;
const BYTE MSB_EDGE_CLOCK_OUT_BYTE = 0x11;
const BYTE MSB_EDGE_CLOCK_IN_BYTE = 0x24;

const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_IN = 0x24;
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT = 0x11;
const BYTE MSB_DOWN_EDGE_CLOCK_BIT_IN = 0x26;
const BYTE MSB_UP_EDGE_CLOCK_BYTE_IN = 0x20;
const BYTE MSB_UP_EDGE_CLOCK_BYTE_OUT = 0x10;
const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN = 0x22;
//--------------------------------------------------------------------------------
I2C::I2C(I2C_Freq freq,
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
void I2C::HighSpeedSetI2CStart(void)
{
    DWORD dwCount;
    for(dwCount=0; dwCount<4; dwCount++)	// Repeat commands to ensure the minimum period of the start hold time ie 600ns is achieved
    {
        OutputBuffer[dwNumBytesToSend++] = 0x80;  // Command to set directions of lower 8 pins and force value on bits set as output
        OutputBuffer[dwNumBytesToSend++] = 0x03;  // Set SDA, SCL high, WP disabled by SK, DO at bit '1', GPIOL0 at bit '0'
        OutputBuffer[dwNumBytesToSend++] = 0x13;  // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    }
    for(dwCount=0; dwCount < 4; dwCount++)	// Repeat commands to ensure the minimum period of the start setup time ie 600ns is achieved
    {
        OutputBuffer[dwNumBytesToSend++] = 0x80;  // Command to set directions of lower 8 pins and force value on bits set as output
        OutputBuffer[dwNumBytesToSend++] = 0x01;  // Set SDA low, SCL high, WP disabled by SK at bit '1', DO, GPIOL0 at bit '0'
        OutputBuffer[dwNumBytesToSend++] = 0x13;  // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    }
    OutputBuffer[dwNumBytesToSend++] = 0x80;      // Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = 0x00;      // Set SDA, SCL low high, WP disabled by SK, DO, GPIOL0 at bit '0'
    OutputBuffer[dwNumBytesToSend++] = 0x13;      // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
}
//--------------------------------------------------------------------------------
void I2C::HighSpeedSetI2CStop(void)
{
    DWORD dwCount;
    for(dwCount=0; dwCount<4; dwCount++)	// Repeat commands to ensure the minimum period of the stop setup time ie 600ns is achieved
    {
        OutputBuffer[dwNumBytesToSend++] = 0x80;  // Command to set directions of lower 8 pins and force value on bits set as output
        OutputBuffer[dwNumBytesToSend++] = 0x01;  // Set SDA low, SCL high, WP disabled by SK at bit '1', DO, GPIOL0 at bit '0'
        OutputBuffer[dwNumBytesToSend++] = 0x13;  // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    }
    for(dwCount=0; dwCount<4; dwCount++)	// Repeat commands to ensure the minimum period of the stop hold time ie 600ns is achieved
    {
        OutputBuffer[dwNumBytesToSend++] = 0x80;  // Command to set directions of lower 8 pins and force value on bits set as output
        OutputBuffer[dwNumBytesToSend++] = 0x03;  // Set SDA, SCL high, WP disabled by SK, DO at bit '1', GPIOL0 at bit '0'
        OutputBuffer[dwNumBytesToSend++] = 0x13;  // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    }
    // Tristate the SCL, SDA pins
    OutputBuffer[dwNumBytesToSend++] = 0x80;      // Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = 0x00;      // Set WP disabled by GPIOL0 at bit 0'
    OutputBuffer[dwNumBytesToSend++] = 0x10;      // Set GPIOL0 pins as output with bit '1', SK, DO and other pins as input with bit '0'
}
//--------------------------------------------------------------------------------
BOOL I2C::SendByteAndCheckACK(BYTE dwDataSend)
{
    FT_STATUS ftStatus = FT_OK;
    OutputBuffer[dwNumBytesToSend++] = MSB_FALLING_EDGE_CLOCK_BYTE_OUT; 	//clock data byte out on -ve Clock Edge MSB first
    OutputBuffer[dwNumBytesToSend++] = 0x00;
    OutputBuffer[dwNumBytesToSend++] = 0x00;      // Data length of 0x0000 means 1 byte data to clock out
    OutputBuffer[dwNumBytesToSend++] = dwDataSend;	// Set control byte, bit 4-7 of '1010' as 24LC02 control code, bit 1-3 as block select bits  //which is don't care here, bit 0 of '0' represent Write operation
    //Get Acknowledge bit from EEPROM
    OutputBuffer[dwNumBytesToSend++] = 0x80;      // Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = 0x00;      // Set SCL low, WP disabled by SK, GPIOL0 at bit '0'
    OutputBuffer[dwNumBytesToSend++] = 0x11;      // Set SK, GPIOL0 pins as output with bit '1', DO and other pins as input with bit '0'
    OutputBuffer[dwNumBytesToSend++] = MSB_RISING_EDGE_CLOCK_BIT_IN; 	//Command to scan in acknowledge bit , -ve clock Edge MSB first
    OutputBuffer[dwNumBytesToSend++] = 0x00;      // Length of 0x0 means to scan in 1 bit

    OutputBuffer[dwNumBytesToSend++] = 0x87;        //Send answer back immediate command
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);		//Send off the commands
    dwNumBytesToSend = 0;                           //Clear output buffer
    //Check if ACK bit received, may need to read more times to get ACK bit or fail if timeout
    ftStatus = FT_Read(ftHandle, InputBuffer, 1, &dwNumBytesRead);  	// Read one byte from device receive buffer
    if((ftStatus != FT_OK) || (dwNumBytesRead == 0))
    {
        print_error("fail to get ACK when send control byte 1 [Program Section]", ftStatus);
        return FALSE; //Error, can't get the ACK bit from EEPROM
    }
    else
    {
        if(((InputBuffer[0] & BYTE(0x1)) != BYTE(0x0)))	//Check ACK bit 0 on data byte read out
        {
            print_error("fail to get ACK when send control byte 2 [Program Section]", ftStatus);
            return FALSE; //Error, can't get the ACK bit from EEPROM
        }
    }

    OutputBuffer[dwNumBytesToSend++] = 0x80;	// Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = 0x02; 	//Set SDA high, SCL low, WP disabled by SK at bit '0', DO, GPIOL0 at bit '1'
    OutputBuffer[dwNumBytesToSend++] = 0x13;  //Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    return TRUE;
}
//--------------------------------------------------------------------------------
bool I2C::VerifyMPSSE(void)
{
    //////////////////////////////////////////////////////////////////
    // Synchronize the MPSSE interface by sending bad command '0xAA'
    //////////////////////////////////////////////////////////////////
    DWORD dwCount;
    OutputBuffer[dwNumBytesToSend++] = 0xAA;		//Add BAD command '0xAA'
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
        if ((InputBuffer[dwCount] == BYTE(0xFA)) && (InputBuffer[dwCount+1] == BYTE(0xAA)))
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
    OutputBuffer[dwNumBytesToSend++] = 0xAB;	//Send BAD command '0xAB'
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
        if ((InputBuffer[dwCount] == BYTE(0xFA)) && (InputBuffer[dwCount+1] == BYTE(0xAB)))
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
void I2C::ConfigureMPSSE(void)
{
    ////////////////////////////////////////////////////////////////////
    // Configure the MPSSE for I2C communication with 24LC02B
    //////////////////////////////////////////////////////////////////
    OutputBuffer[dwNumBytesToSend++] = 0x8A;  //Ensure disable clock divide by 5 for 60Mhz master clock
    OutputBuffer[dwNumBytesToSend++] = 0x97;	//Ensure turn off adaptive clocking
    OutputBuffer[dwNumBytesToSend++] = 0x8D; 	//Enable 3 phase data clock, used by I2C to allow data on both clock edges
    // Send off the commands
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("ConfigureMPSSE: FT_Write(1)", ftStatus);
    }
    dwNumBytesToSend = 0;                       //Clear output buffer
    OutputBuffer[dwNumBytesToSend++] = 0x80;	// Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = 0x03; 	//Set SDA, SCL high, WP disabled by SK, DO at bit '1', GPIOL0 at bit '0'
    OutputBuffer[dwNumBytesToSend++] = 0x13;  //Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    // The SK clock frequency can be worked out by below algorithm with divide by 5 set as off
    // SK frequency  = 60MHz /((1 +  [(1 +0xValueH*256) OR 0xValueL])*2)
    OutputBuffer[dwNumBytesToSend++] = 0x86;                            //Command to set clock divisor
    OutputBuffer[dwNumBytesToSend++] = dwClockDivisor & 0xFF;           //Set 0xValueL of clock divisor
    OutputBuffer[dwNumBytesToSend++] = (dwClockDivisor >> 8) & 0xFF;	  //Set 0xValueH of clock divisor
    // Send off the commands
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("ConfigureMPSSE: FT_Write(2)", ftStatus);
    }
    dwNumBytesToSend = 0;		//Clear output buffer
    Sleeper::msleep(20);		//Delay for a while

    //Turn off loop back in case
    OutputBuffer[dwNumBytesToSend++] = 0x85;		//Command to turn off loop back of TDI/TDO connection
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the commands
    if(ftStatus != FT_OK)
    {
        emit error(QString("ConfigureMPSSE: FT_Write(3) return %1").arg(ftStatus));
    }
    dwNumBytesToSend = 0;		//Clear output buffer
    Sleeper::msleep(30);		//Delay for a while
}
//--------------------------------------------------------------------------------
bool I2C::write(unsigned char id,
                unsigned char address,
                unsigned char data)
{
    bool bSucceed = false;

#if 0
    emit debug(QString("write: id %1 address %2 data %3")
               .arg(id)
               .arg(address)
               .arg(data));
#endif

    dwNumBytesToSend = 0;

    HighSpeedSetI2CStart();

    bSucceed = SendByteAndCheckACK(id);
    if(bSucceed == FALSE)
    {
        emit error(QString("Error send ID (%1)").arg(id));
        return false;
    }

    //    bSucceed = SendByteAndCheckACK(addr.bytes.hi);
    //    if(bSucceed == FALSE)
    //    {
    //        emit error(QString("Error send address(HI) (%1)").arg(addr.bytes.hi));
    //        return false;
    //    }

    bSucceed = SendByteAndCheckACK(address);
    if(bSucceed == FALSE)
    {
        emit error(QString("Error send address (%1)").arg(address));
        return false;
    }

    bSucceed = SendByteAndCheckACK(data);
    if(bSucceed == FALSE)
    {
        emit error(QString("Error send data (%1)").arg(data));
        return false;
    }

    HighSpeedSetI2CStop();

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("write", ftStatus);
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool I2C::write_one_byte(unsigned char address,
                         unsigned char data)
{
    bool bSucceed = false;

#if 0
    emit debug(QString("write: id %1 address %2 data %3")
               .arg(id)
               .arg(address)
               .arg(data));
#endif

    dwNumBytesToSend = 0;

    HighSpeedSetI2CStart();

    bSucceed = SendByteAndCheckACK(address);
    if(bSucceed == FALSE)
    {
        emit error(QString("Error send address (%1)").arg(address));
        return false;
    }

    bSucceed = SendByteAndCheckACK(data);
    if(bSucceed == FALSE)
    {
        emit error(QString("Error send data (%1)").arg(data));
        return false;
    }

    HighSpeedSetI2CStop();

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("write", ftStatus);
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool I2C::read(unsigned char id,
               unsigned char address,
               unsigned char *data)
{
    bool bSucceed = false;

#if 0
    emit debug(QString("address %1")
               .arg(address));
#endif

    // Purge USB receive buffer first before read operation
    ftStatus = FT_GetQueueStatus(ftHandle, &dwNumInputBuffer);	 // Get the number of bytes in the device receive buffer
    if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
        FT_Read(ftHandle, &InputBuffer, dwNumInputBuffer, &dwNumBytesRead);  //Read out all the data from receive buffer

    dwNumBytesToSend = 0;

    HighSpeedSetI2CStart();

    bSucceed = SendByteAndCheckACK(id);
    if(bSucceed == FALSE)
    {
        emit error(QString("Error send ID (%1)").arg(id));
        return false;
    }


    //    bSucceed = SendByteAndCheckACK(addr.bytes.hi);
    //    if(bSucceed == FALSE)
    //    {
    //        emit error(QString("Error send address(HI) (%1)").arg(addr.bytes.hi));
    //        return false;
    //    }

    bSucceed = SendByteAndCheckACK(address);
    if(bSucceed == FALSE)
    {
        print_error("SendByteAndCheckACK(address)", address);
        return false;
    }

    HighSpeedSetI2CStart();

    id = id | 0x1;
    bSucceed = SendByteAndCheckACK(id);
    if(bSucceed == FALSE)
    {
        print_error("SendByteAndCheckACK(id)", address);
        return false;
    }

    //////////////////////////////////////////////////////////
    // Read the data from 24LC02B with no ACK bit check
    //////////////////////////////////////////////////////////
    OutputBuffer[dwNumBytesToSend++] = 0x80;        // Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = 0x00;        // Set SCL low, WP disabled by SK, GPIOL0 at bit '0'
    OutputBuffer[dwNumBytesToSend++] = 0x11;        // Set SK, GPIOL0 pins as output with bit '1', DO and other pins as input with bit '0'
    OutputBuffer[dwNumBytesToSend++] = MSB_FALLING_EDGE_CLOCK_BYTE_IN; 	//Command to clock data byte in on -ve Clock Edge MSB first
    OutputBuffer[dwNumBytesToSend++] = 0x00;
    OutputBuffer[dwNumBytesToSend++] = 0x00;        // Data length of 0x0000 means 1 byte data to clock in
    //OutputBuffer[dwNumBytesToSend++] = 0x87;      // Send answer back immediate command

    OutputBuffer[dwNumBytesToSend++] = MSB_RISING_EDGE_CLOCK_BIT_IN; 	//Command to scan in acknowledge bit , -ve clock Edge MSB first
    OutputBuffer[dwNumBytesToSend++] = 0x00;        // Length of 0 means to scan in 1 bit
    OutputBuffer[dwNumBytesToSend++] = 0x87;        // Send answer back immediate command

    // Send off the commands
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("Send off the commands failed (1) FT_Write", ftStatus);
        return false;
    }

    dwNumBytesToSend = 0;                                                               // Clear output buffer

    // Read one byte from device receive buffer, may need try more times
    ftStatus = FT_Read(ftHandle, InputBuffer, 2, &dwNumBytesRead);
    if(ftStatus != FT_OK)
    {
        print_error("Read data failed! FT_Read", ftStatus);
        return false;
    }

    //qDebug() << "InputBuffer[0]" << InputBuffer[0] << "InputBuffer[1]" << InputBuffer[1];
    *data = InputBuffer[0]; // return the data read from EEPROM

    OutputBuffer[dwNumBytesToSend++] = 0x80;     // Command to set directions of lower 8 pins and force value on bits set as output
    OutputBuffer[dwNumBytesToSend++] = 0x02;     // Set SDA high, SCL low, WP disabled by SK at bit '0', DO, GPIOL0 at bit '1'
    OutputBuffer[dwNumBytesToSend++] = 0x13;     // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'

    HighSpeedSetI2CStop();

    //Send off the commands
    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        print_error("Send off the commands failed (2) FT_Write", ftStatus);
        return false;
    }

    return true;
}
//--------------------------------------------------------------------------------
bool I2C::open(int deviceNumber)
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
    }
    ftStatus = FT_ListDevices((PVOID)devIndex ,&Buf, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
    if (ftStatus != FT_OK)
    {
        print_error("Can't open FT2232H device! FT_ListDevices(2)", ftStatus);
    }

    emit info(QString("Open device #%1").arg(deviceNumber));
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
void I2C::close(void)
{
    ftStatus = FT_Close(ftHandle);
    if(ftStatus == FT_OK)
        ftHandle = 0;
}
//--------------------------------------------------------------------------------
void I2C::test_read(void)
{
    emit info("test_read begin");

    for(addr=0; addr<MAX_ADDRESS; addr++)
    {
        QCoreApplication::processEvents();
        data = 0;
        bool ok = read(160, addr, &data);
        if(!ok) break;

        if(addr == data)
        {
            emit info(QString("read address %1 data %2")
                      .arg(addr, 0, 16)
                      .arg(data, 0, 16));
        }
        else
        {
            emit error(QString("read address %1 != data %2")
                       .arg(addr, 0, 16)
                       .arg(data, 0, 16));
        }
        Sleeper::msleep(10);
    }

    emit info("test_read end");
}
//--------------------------------------------------------------------------------
void I2C::test_write(void)
{
    emit info("test_write begin");

    bool ok = false;
    for(addr=0; addr<MAX_ADDRESS; addr++)
    {
        QCoreApplication::processEvents();
        ok = write(160, addr, addr);
        if(!ok) break;

        emit info(QString("write address %1 data %2")
                  .arg(addr)
                  .arg(addr));
        Sleeper::msleep(50);
    }

    emit info("test_write end");
}
//--------------------------------------------------------------------------------
void I2C::clear_all(void)
{
    emit info("clear_all begin");

    bool ok = false;
    for(addr=0; addr<MAX_ADDRESS; addr++)
    {
        QCoreApplication::processEvents();
        ok = write(160, addr, 0xFF);
        if(!ok) break;

        emit info(QString("write address %1 data %2")
                  .arg(addr)
                  .arg(addr));
        Sleeper::msleep(50);
    }

    emit info("clear_all end");
}
//--------------------------------------------------------------------------------
void I2C::test(void)
{
    bool ok = write_one_byte(0x4, 0x55);
    //bool ok = write_one_byte(1, 0x55);
    if(ok)
    {
        emit info("test OK");
    }
    else
    {
        emit error("test failed");
    }
}
//--------------------------------------------------------------------------------
void I2C::print_error(const QString &function,
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
void I2C::updateText(void)
{

}
//--------------------------------------------------------------------------------
