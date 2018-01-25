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
#include <QDebug>
//--------------------------------------------------------------------------------
#include <ftd2xx.h>
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#include "spi.hpp"
//--------------------------------------------------------------------------------
//declare parameters for 93C56
const BYTE SPIDATALENGTH = 11;  // 3 digit command + 8 digit address
const BYTE READ  = '\xC0';      // 110xxxxx
const BYTE WRITE = '\xA0';      // 101xxxxx
const BYTE WREN  = '\x98';      // 10011xxx
const BYTE ERAL  = '\x90';      // 10010xxx

//declare for MPSSE command
const BYTE MSB_RISING_EDGE_CLOCK_BYTE_OUT   = '\x10';
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT  = '\x11';
const BYTE MSB_RISING_EDGE_CLOCK_BIT_OUT    = '\x12';
const BYTE MSB_FALLING_EDGE_CLOCK_BIT_OUT   = '\x13';
const BYTE MSB_RISING_EDGE_CLOCK_BYTE_IN    = '\x20';
const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN     = '\x22';
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_IN   = '\x24';
const BYTE MSB_FALLING_EDGE_CLOCK_BIT_IN    = '\x26';
//--------------------------------------------------------------------------------
SPI::SPI(I2C_Freq freq,
         QWidget *parent) :
    MyWidget(parent)
{
    dwClockDivisor = 29;  // Value of clock divisor, SCL Frequency = 60/((1+29)*2) (MHz) = 1Mhz
    ft2232h = new FT2232H(freq, parent);
}
//--------------------------------------------------------------------------------
//this routine is used to enable SPI device
void SPI::SPI_CSEnable(void)
{
    for(int loop=0; loop<5; loop++) // one 0x80 command can keep 0.2us, do 5 times to stay in this situation for 1us
    {
        ft2232h->append_data('\x80');  // GPIO command for ADBUS
        ft2232h->append_data('\x08');  // set CS high, MOSI and SCL low
        ft2232h->append_data('\x0b');  // bit3:CS, bit2:MISO, bit1:MOSI, bit0:SCK
    }
}
//--------------------------------------------------------------------------------
//this routine is used to disable SPI device
void SPI::SPI_CSDisable(void)
{
    for(int loop=0; loop<5; loop++) // one 0x80 command can keep 0.2us, do 5 times to stay in this situation for 1us
    {
        ft2232h->append_data('\x80');  // GPIO command for ADBUS
        ft2232h->append_data('\x00');  // set CS, MOSI and SCL low
        ft2232h->append_data('\x0b');  // bit3:CS, bit2:MISO, bit1:MOSI, bit0:SCK
    }
}
//--------------------------------------------------------------------------------
//this routine is used to send command to 93C56 EEPROM
FT_STATUS SPI::WriteEECmd(BYTE command)
{
    ft2232h->clear_data();
    SPI_CSEnable();
    // SPIDATALENGTH = 11, it can be divide into 8+3 bits
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7); // 7+1 = 8
    ft2232h->append_data(command);
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(SPIDATALENGTH - (8+1));
    ft2232h->append_data('\xFF');
    SPI_CSDisable();
    return ft2232h->write_data();
}
//--------------------------------------------------------------------------------
//this routine is used to initial SPI interface
BOOL SPI::SPI_Initial(DWORD dwClockDivisor)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD dwNumInputBuffer = 0;
    DWORD dwNumBytesRead = 0;
    DWORD dwCount = 0;
    BYTE buf[1024];

    ftStatus = ft2232h->resetDevice(); // Reset USB device
    // Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
    ftStatus |= ft2232h->getQueueStatus(&dwNumInputBuffer); // Get the number of bytes in the FT2232H receive buffer
    if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
        ftStatus |= ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead); //Read out the data from FT2232H receive buffer
    ftStatus |= ft2232h->setUSBParameters(65535, 65535);    // Set USB request transfer size
    ftStatus |= ft2232h->setChars(false, 0, false, 0);      // Disable event and error characters
    ftStatus |= ft2232h->setTimeouts(3000, 3000);           // Sets the read and write timeouts in 3 sec for the FT2232H
    ftStatus |= ft2232h->setLatencyTimer(1);                // Set the latency timer
    ftStatus |= ft2232h->setBitMode(0x0, 0x00);             // Reset controller
    ftStatus |= ft2232h->setBitMode(0x0, 0x02);             // Enable MPSSE mode
    if (ftStatus != FT_OK)
    {
        ft2232h->print_error("fail on initialize FT2232H device: functions", ftStatus);
        return false;
    }
    Sleeper::msleep(50); // Wait 50ms for all the USB stuff to complete and work
    //////////////////////////////////////////////////////////////////
    // Synchronize the MPSSE interface by sending bad command &xAA*
    //////////////////////////////////////////////////////////////////
    ft2232h->clear_data();
    ft2232h->append_data('\xAA');       // Add BAD command &xAA*
    ftStatus = ft2232h->write_data();   // Send off the BAD commands
    ft2232h->clear_data();              // Clear output buffer
    do{
        ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);  // Get the number of bytes in the device input buffer
    }while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK));        // or Timeout
    bool bCommandEchod = false;
    ftStatus = ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead); // Read out the data from input buffer
    for (dwCount = 0; dwCount < (dwNumBytesRead - 1); dwCount++)    // Check if Bad command and echo command received
    {
        if ((buf[dwCount] == BYTE('\xFA')) && (buf[dwCount+1] == BYTE('\xAA')))
        {
            bCommandEchod = true;
            break;
        }
    }
    if (bCommandEchod == false)
    {
        emit error("fail to synchronize MPSSE with command '0xAA'");
        return false; /*Error, can*t receive echo command , fail to synchronize MPSSE interface;*/
    }
    //////////////////////////////////////////////////////////////////
    // Synchronize the MPSSE interface by sending bad command &xAB*
    //////////////////////////////////////////////////////////////////
    //ft2232h->clear_data();              // Clear output buffer
    ft2232h->append_data('\xAB');       // Send BAD command &xAB*
    ftStatus = ft2232h->write_data();   // Send off the BAD commands
    ft2232h->clear_data();              // Clear output buffer
    do{
        ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);  // Get the number of bytes in the device input buffer
    }while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK));        // or Timeout
    bCommandEchod = false;
    ftStatus = ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead);   // Read out the data from input buffer
    for (dwCount = 0;dwCount < (dwNumBytesRead - 1); dwCount++)                     // Check if Bad command and echo command received
    {
        if ((buf[dwCount] == BYTE('\xFA')) && (buf[dwCount+1] == BYTE('\xAB')))
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
    ////////////////////////////////////////////////////////////////////
    //Configure the MPSSE for SPI communication with EEPROM
    //////////////////////////////////////////////////////////////////
    ft2232h->append_data('\x8A');       // Ensure disable clock divide by 5 for 60Mhz master clock
    ft2232h->append_data('\x97');       // Ensure turn off adaptive clocking
    ft2232h->append_data('\x8D');       // disable 3 phase data clock
    ftStatus = ft2232h->write_data();   // Send out the commands
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("SPI_Initial: FT_Write(1)", ftStatus);
    }

    ft2232h->clear_data();  // Clear output buffer
    ft2232h->append_data('\x80');  // Command to set directions of lower 8 pins and force value on bits set as output
    ft2232h->append_data('\x00');  // Set SDA, SCL high, WP disabled by SK, DO at bit &*, GPIOL0 at bit &*
    ft2232h->append_data('\x0b');  // Set SK,DO,GPIOL0 pins as output with bit **, other pins as input with bit &*
    // The SK clock frequency can be worked out by below algorithm with divide by 5 set as off
    // SK frequency = 60MHz /((1 + [(1 +0xValueH*256) OR 0xValueL])*2)
    ft2232h->append_data('\x86');  // Command to set clock divisor
    ft2232h->append_data(BYTE(dwClockDivisor & '\xFF'));    // Set 0xValueL of clock divisor
    ft2232h->append_data(BYTE(dwClockDivisor >> 8));        // Set 0xValueH of clock divisor
    ftStatus = ft2232h->write_data();                       // Send out the commands
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("SPI_Initial: FT_Write(2)", ftStatus);
    }

    Sleeper::msleep(20);    // Delay for a while
    //Turn off loop back in case
    ft2232h->clear_data();              // Clear output buffer
    ft2232h->append_data('\x85');       // Command to turn off loop back of TDI/TDO connection
    ftStatus = ft2232h->write_data();   // Send out the commands
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("SPI_Initial: FT_Write(3)", ftStatus);
    }

    ft2232h->clear_data();  // Clear output buffer
    Sleeper::msleep(30);    // Delay for a while
    emit info("SPI initial successful");
    return true;
}
//--------------------------------------------------------------------------------
//this routine is used to write one word data to a random address
BOOL SPI::SPI_WriteByte2RandomAddr(WORD address, WORD bdata)
{
    FT_STATUS ftStatus = FT_OK;

    ft2232h->clear_data();
    SPI_CSEnable();
    // send WRITE command
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(2);
    ft2232h->append_data(WRITE);
    // send address
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7);
    ft2232h->append_data((BYTE)(address));
    // send data
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BYTE_OUT);
    ft2232h->append_data(1);
    ft2232h->append_data(0);            // Data length of 0x0001 means 2 byte data to clock out
    ft2232h->append_data(bdata >> 8);   // output high byte
    ft2232h->append_data(bdata & 0xff); // output low byte
    SPI_CSDisable();
    ftStatus = ft2232h->write_data();   // send out MPSSE command to MPSSE engine
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("SPI_WriteByte2RandomAddr: FT_Write", ftStatus);
    }
    ft2232h->clear_data();              // Clear output buffer
    return ftStatus;
}
//--------------------------------------------------------------------------------
//this routine is used to read one word data from a random address
BOOL SPI::SPI_ReadByteRandomAddr(WORD address, WORD* bdata)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD dwNumBytesRead = 0;
    BYTE buf[1024];

    ft2232h->clear_data();
    SPI_CSEnable();
    // send WRITE command
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(2);
    ft2232h->append_data(READ);
    // send address
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7);
    ft2232h->append_data((BYTE)(address));
    //read data
    ft2232h->append_data( MSB_FALLING_EDGE_CLOCK_BYTE_IN);
    ft2232h->append_data('\x01');
    ft2232h->append_data('\x00');       //Data length of 0x0001 means 2 byte data to clock in
    SPI_CSDisable();
    ftStatus = ft2232h->write_data();   // send out MPSSE command to MPSSE engine
    ft2232h->clear_data();              // Clear output buffer
    ftStatus = ft2232h->read(&buf[0], 2, &dwNumBytesRead);  // Read 2 bytes from device receive buffer
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("SPI_ReadByteRandomAddr: FT_Read", ftStatus);
    }
    *bdata = (buf[0] << 8) + buf[1];
    return ftStatus;
}
//--------------------------------------------------------------------------------
BOOL SPI::test_at93c56(void)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD dwNumInputBuffer = 0;
    DWORD dwNumBytesRead = 0;
    BYTE buf[1024];
    WORD DataInBuffer[MemSize];
    BYTE DataOutBuffer[MemSize];

    BOOL ok = open();
    if(ok == FALSE)
        return FALSE;

    // initial output buffer
    for(int i=0; i<MemSize; i++)
        DataOutBuffer[i] = i;

    if(SPI_Initial(dwClockDivisor) == TRUE)
    {
        // Purge USB received buffer first before read operation
        ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);    // Get the number of bytes in the device receive buffer
        if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
            ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead);    // Read out all the data from receive buffer
        WriteEECmd(WREN);
        WriteEECmd(ERAL);
        Sleeper::msleep(20);
        for(int i=0; i<MemSize; i++)
        {
            SPI_WriteByte2RandomAddr(i, DataOutBuffer[i]);
            Sleeper::msleep(2);
            emit info(QString("Write data %1 to address 0x%2")
                      .arg(DataOutBuffer[i])
                      .arg(i, 0, 16));
        }
        Sleeper::msleep(20);
        for(int i=0; i<MemSize; i++)
        {
            SPI_ReadByteRandomAddr(i, &DataInBuffer[i]);
            emit info(QString("Read data from address %1 = 0x%2")
                      .arg(i)
                      .arg(DataInBuffer[i], 0, 16));
        }
    }

    ok = close();
    return ok;
}
//--------------------------------------------------------------------------------
BOOL SPI::open(void)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD numDevs = 0;
    FT_DEVICE_LIST_INFO_NODE *devInfo;

    ftStatus = ft2232h->createDeviceInfoList(&numDevs);
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("open", ftStatus);
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
    devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
    // get the device information list
    ftStatus = FT_GetDeviceInfoList(devInfo,&numDevs);
    if (ftStatus != FT_OK)
    {
        ft2232h->print_error("open: FT_GetDeviceInfoList", ftStatus);
        return FALSE;
    }

    for(DWORD i=0; i<numDevs; i++)
    {
        emit info(QString("Dev %1").arg(i));
        emit info(QString(" Flags=0x%1").arg(devInfo[i].Flags, 0, 16));
        emit info(QString(" Type=0x%1").arg(devInfo[i].Type, 0, 16));
        emit info(QString(" ID=0x%1").arg(devInfo[i].ID, 0, 16));
        emit info(QString(" LocId=0x%1").arg(devInfo[i].LocId, 0, 16));
        emit info(QString(" SerialNumber=%1").arg(devInfo[i].SerialNumber));
        emit info(QString(" Description=%1").arg(devInfo[i].Description));
        //emit info(QString(" ftHandle=0x%1").arg(*devInfo[i].ftHandle));
    }

    ftStatus = ft2232h->open(0);
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("Can't open FT2232H device: FT_Open", ftStatus);
        return FALSE;
    }

    emit info("Successfully open FT2232H device!");
    return TRUE;
}
//--------------------------------------------------------------------------------
BOOL SPI::close(void)
{
    FT_STATUS ftStatus = FT_OK;

    ftStatus = ft2232h->close();
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("close", ftStatus);
        return FALSE;
    }
    return TRUE;
}
//--------------------------------------------------------------------------------
BOOL SPI::write(WORD address, WORD bdata)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD dwNumInputBuffer = 0;
    DWORD dwNumBytesRead = 0;
    BYTE buf[1024];

    // Purge USB received buffer first before read operation
    ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);    // Get the number of bytes in the device receive buffer
    if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
        ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead);    // Read out all the data from receive buffer
    WriteEECmd(WREN);
    WriteEECmd(ERAL);
    Sleeper::msleep(20);
    return SPI_WriteByte2RandomAddr(address, bdata);
}
//--------------------------------------------------------------------------------
BOOL SPI::read(WORD address, WORD* bdata)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD dwNumInputBuffer = 0;
    DWORD dwNumBytesRead = 0;
    BYTE buf[1024];

    // Purge USB received buffer first before read operation
    ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);    // Get the number of bytes in the device receive buffer
    if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
        ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead);    // Read out all the data from receive buffer
    WriteEECmd(WREN);
    WriteEECmd(ERAL);
    Sleeper::msleep(20);
    return SPI_ReadByteRandomAddr(address, bdata);
}
//--------------------------------------------------------------------------------
void SPI::updateText(void)
{

}
//--------------------------------------------------------------------------------
