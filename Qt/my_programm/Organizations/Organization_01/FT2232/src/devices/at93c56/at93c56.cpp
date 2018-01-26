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
//--------------------------------------------------------------------------------
#include "ft2232h.hpp"
#include "sleeper.h"
#include "at93c56.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
const BYTE MSB_VEDGE_CLOCK_IN_BIT = '\x22';
const BYTE MSB_EDGE_CLOCK_OUT_BYTE = '\x11';
const BYTE MSB_EDGE_CLOCK_IN_BYTE = '\x24';

const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_IN = '\x24';
const BYTE MSB_FALLING_EDGE_CLOCK_BYTE_OUT = '\x11';
const BYTE MSB_DOWN_EDGE_CLOCK_BIT_IN = '\x26';
const BYTE MSB_UP_EDGE_CLOCK_BYTE_IN = '\x20';
const BYTE MSB_UP_EDGE_CLOCK_BYTE_OUT = '\x10';
const BYTE MSB_RISING_EDGE_CLOCK_BIT_IN = '\x22';
//--------------------------------------------------------------------------------
AT93C56::AT93C56(I2C_Freq freq,
                 QWidget *parent) :
    MyWidget(parent),
    ft2232h(0)
{
    ft2232h = new FT2232H(freq, parent);
}
//--------------------------------------------------------------------------------
AT93C56::~AT93C56(void)
{
    ft2232h->close();
}
//--------------------------------------------------------------------------------
void AT93C56::HighSpeedSetI2CStart(void)
{
    DWORD dwCount;
    for(dwCount=0; dwCount<4; dwCount++)	// Repeat commands to ensure the minimum period of the start hold time ie 600ns is achieved
    {
        ft2232h->append_data('\x80');  // Command to set directions of lower 8 pins and force value on bits set as output
        ft2232h->append_data('\x03');  // Set SDA, SCL high, WP disabled by SK, DO at bit '1', GPIOL0 at bit '0'
        ft2232h->append_data('\x13');  // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    }
    for(dwCount=0; dwCount < 4; dwCount++)	// Repeat commands to ensure the minimum period of the start setup time ie 600ns is achieved
    {
        ft2232h->append_data('\x80');  // Command to set directions of lower 8 pins and force value on bits set as output
        ft2232h->append_data('\x01');  // Set SDA low, SCL high, WP disabled by SK at bit '1', DO, GPIOL0 at bit '0'
        ft2232h->append_data('\x13');  // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    }
    ft2232h->append_data('\x80');      // Command to set directions of lower 8 pins and force value on bits set as output
    ft2232h->append_data('\x00');      // Set SDA, SCL low high, WP disabled by SK, DO, GPIOL0 at bit '0'
    ft2232h->append_data('\x13');      // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
}
//--------------------------------------------------------------------------------
void AT93C56::HighSpeedSetI2CStop(void)
{
    DWORD dwCount;
    for(dwCount=0; dwCount<4; dwCount++)	// Repeat commands to ensure the minimum period of the stop setup time ie 600ns is achieved
    {
        ft2232h->append_data('\x80');  // Command to set directions of lower 8 pins and force value on bits set as output
        ft2232h->append_data('\x01');  // Set SDA low, SCL high, WP disabled by SK at bit '1', DO, GPIOL0 at bit '0'
        ft2232h->append_data('\x13');  // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    }
    for(dwCount=0; dwCount<4; dwCount++)	// Repeat commands to ensure the minimum period of the stop hold time ie 600ns is achieved
    {
        ft2232h->append_data('\x80');  // Command to set directions of lower 8 pins and force value on bits set as output
        ft2232h->append_data('\x03');  // Set SDA, SCL high, WP disabled by SK, DO at bit '1', GPIOL0 at bit '0'
        ft2232h->append_data('\x13');  // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    }
    // Tristate the SCL, SDA pins
    ft2232h->append_data('\x80');      // Command to set directions of lower 8 pins and force value on bits set as output
    ft2232h->append_data('\x00');      // Set WP disabled by GPIOL0 at bit 0'
    ft2232h->append_data('\x10');      // Set GPIOL0 pins as output with bit '1', SK, DO and other pins as input with bit '0'
}
//--------------------------------------------------------------------------------
BOOL AT93C56::SendByteAndCheckACK(BYTE dwDataSend)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD dwNumBytesRead = 0;
    BYTE buf[1024];

    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BYTE_OUT); 	//clock data byte out on -ve Clock Edge MSB first
    ft2232h->append_data('\x00');
    ft2232h->append_data('\x00');       // Data length of 0x0000 means 1 byte data to clock out
    ft2232h->append_data(dwDataSend);   // Set control byte, bit 4-7 of '1010' as 24LC02 control code, bit 1-3 as block select bits  //which is don't care here, bit 0 of '0' represent Write operation
    //Get Acknowledge bit from EEPROM
    ft2232h->append_data('\x80');       // Command to set directions of lower 8 pins and force value on bits set as output
    ft2232h->append_data('\x00');       // Set SCL low, WP disabled by SK, GPIOL0 at bit '0'
    ft2232h->append_data('\x11');       // Set SK, GPIOL0 pins as output with bit '1', DO and other pins as input with bit '0'
    ft2232h->append_data(MSB_RISING_EDGE_CLOCK_BIT_IN); 	//Command to scan in acknowledge bit , -ve clock Edge MSB first
    ft2232h->append_data('\x00');       // Length of 0x0 means to scan in 1 bit

    ft2232h->append_data('\x87');       //Send answer back immediate command
    ft2232h->write_data();

    ft2232h->clear_data();              //Clear output buffer
    //Check if ACK bit received, may need to read more times to get ACK bit or fail if timeout
    ftStatus = ft2232h->read(&buf[0], 1, &dwNumBytesRead);  	// Read one byte from device receive buffer
    if((ftStatus != FT_OK) || (dwNumBytesRead == 0))
    {
        ft2232h->print_error("fail to get ACK when send control byte 1 [Program Section]", ftStatus);
        return FALSE; //Error, can't get the ACK bit from EEPROM
    }
    else
    {
        if(((buf[0] & BYTE('\x1')) != BYTE('\x0')))	//Check ACK bit 0 on data byte read out
        {
            ft2232h->print_error("fail to get ACK when send control byte 2 [Program Section]", ftStatus);
            return FALSE; //Error, can't get the ACK bit from EEPROM
        }
    }

    ft2232h->append_data('\x80');	// Command to set directions of lower 8 pins and force value on bits set as output
    ft2232h->append_data('\x02'); 	//Set SDA high, SCL low, WP disabled by SK at bit '0', DO, GPIOL0 at bit '1'
    ft2232h->append_data('\x13');   //Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'
    return TRUE;
}
//--------------------------------------------------------------------------------
FT_STATUS AT93C56::write(unsigned char id,
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

    ft2232h->clear_data();

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

    return ft2232h->write_data();
}
//--------------------------------------------------------------------------------
FT_STATUS AT93C56::write_one_byte(unsigned char address,
                                  unsigned char data)
{
    bool bSucceed = false;

#if 0
    emit debug(QString("write: id %1 address %2 data %3")
               .arg(id)
               .arg(address)
               .arg(data));
#endif

    ft2232h->clear_data();

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

    return ft2232h->write_data();
}
//--------------------------------------------------------------------------------
FT_STATUS AT93C56::read(unsigned char id,
                        unsigned char address,
                        unsigned char *data)
{
    bool bSucceed = false;

#if 0
    emit debug(QString("address %1")
               .arg(address));
#endif

    FT_STATUS ftStatus = FT_OK;
    BYTE buf[1024];
    DWORD dwNumBytesRead = 0;
    DWORD dwNumInputBuffer = 0;

    // Purge USB receive buffer first before read operation
    ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);	 // Get the number of bytes in the device receive buffer
    if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
        ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead);  //Read out all the data from receive buffer

    ft2232h->clear_data();

    HighSpeedSetI2CStart();

    bSucceed = SendByteAndCheckACK(id);
    if(bSucceed == FALSE)
    {
        emit error(QString("Error send ID (%1)").arg(id));
        return FT_IO_ERROR;
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
        emit error(QString("SendByteAndCheckACK(address) %1")
                   .arg(address));
        return FT_IO_ERROR;
    }

    HighSpeedSetI2CStart();

    id = id | 0x1;
    bSucceed = SendByteAndCheckACK(id);
    if(bSucceed == FALSE)
    {
        emit error(QString("SendByteAndCheckACK(id) %1")
                   .arg(id));
        return FT_IO_ERROR;
    }

    //////////////////////////////////////////////////////////
    // Read the data from 24LC02B with no ACK bit check
    //////////////////////////////////////////////////////////
    ft2232h->append_data('\x80');        // Command to set directions of lower 8 pins and force value on bits set as output
    ft2232h->append_data('\x00');        // Set SCL low, WP disabled by SK, GPIOL0 at bit '0'
    ft2232h->append_data('\x11');        // Set SK, GPIOL0 pins as output with bit '1', DO and other pins as input with bit '0'
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BYTE_IN); 	//Command to clock data byte in on -ve Clock Edge MSB first
    ft2232h->append_data('\x00');
    ft2232h->append_data('\x00');        // Data length of 0x0000 means 1 byte data to clock in
    //ft2232h->append_data('\x87');      // Send answer back immediate command

    ft2232h->append_data(MSB_RISING_EDGE_CLOCK_BIT_IN); 	//Command to scan in acknowledge bit , -ve clock Edge MSB first
    ft2232h->append_data('\x00');        // Length of 0 means to scan in 1 bit
    ft2232h->append_data('\x87');        // Send answer back immediate command

    // Send off the commands
    ftStatus = ft2232h->write_data();
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("Read data failed! FT_Write", ftStatus);
        return FT_IO_ERROR;
    }

    ft2232h->clear_data();               // Clear output buffer

    // Read one byte from device receive buffer, may need try more times
    ftStatus = ft2232h->read(&buf[0], 2, &dwNumBytesRead);
    if(ftStatus != FT_OK)
    {
        ft2232h->print_error("Read data failed! FT_Read", ftStatus);
        return FT_IO_ERROR;
    }

    //qDebug() << "InputBuffer[0]" << InputBuffer[0] << "InputBuffer[1]" << InputBuffer[1];
    *data = buf[0]; // return the data read from EEPROM

    ft2232h->append_data('\x80');     // Command to set directions of lower 8 pins and force value on bits set as output
    ft2232h->append_data('\x02');     // Set SDA high, SCL low, WP disabled by SK at bit '0', DO, GPIOL0 at bit '1'
    ft2232h->append_data('\x13');     // Set SK,DO,GPIOL0 pins as output with bit '1', other pins as input with bit '0'

    HighSpeedSetI2CStop();

    //Send off the commands
    return ft2232h->write_data();
}
//--------------------------------------------------------------------------------
FT_STATUS AT93C56::open(int deviceNumber)
{
    return ft2232h->open(deviceNumber);
}
//--------------------------------------------------------------------------------
void AT93C56::close(void)
{
    ft2232h->close();
}
//--------------------------------------------------------------------------------
void AT93C56::test_read(void)
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
void AT93C56::test_write(void)
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
void AT93C56::clear_all(void)
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
BOOL AT93C56::test(void)
{
    bool ok = write_one_byte(0x4, 0x55);
    //bool ok = write_one_byte(1, 0x55);
    if(!ok)
    {
        emit error("test failed");
        return FALSE;
    }
    emit info("test OK");
    return TRUE;
}
//--------------------------------------------------------------------------------
void AT93C56::updateText(void)
{

}
//--------------------------------------------------------------------------------
