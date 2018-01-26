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
#include "ft2232h.hpp"
#include "mcp4922.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
union MCP4922_DATA
{
    unsigned short value;
    struct
    {
        unsigned DATA:12;   // data 12-bit
        unsigned SHDN:1;    //
        unsigned GA:1;
        unsigned BUF:1;
        unsigned AB:1;
    } bits;
};
//--------------------------------------------------------------------------------
#define DIR_SPI '\x13'  // bit4:CS, bit3:XXX, bit2:MISO, bit1:MOSI, bit0:SCK
//--------------------------------------------------------------------------------
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
MCP4922::MCP4922(I2C_Freq freq,
                 QWidget *parent) :
    MyWidget(parent)
{
    dwClockDivisor = 29;  // Value of clock divisor, SCL Frequency = 60/((1+29)*2) (MHz) = 1Mhz
    ft2232h = new FT2232H(freq, parent);
}
//--------------------------------------------------------------------------------
MCP4922::~MCP4922()
{
    close();
    ft2232h->close();
    ft2232h->deleteLater();
}
//--------------------------------------------------------------------------------
BOOL MCP4922::init(void)
{
    FT_STATUS ftStatus = FT_OK;

    ftStatus = ft2232h->open(1);
    if(ftStatus != FT_OK)
        return FALSE;

    if(SPI_Initial(dwClockDivisor) != TRUE)
    {
        emit error("SPI_Initial return FALSE");
        return FALSE;
    }
    return TRUE;
}
//--------------------------------------------------------------------------------
//this routine is used to enable SPI device
void MCP4922::SPI_CSEnable(void)
{
    for(int loop=0; loop<5; loop++) // one 0x80 command can keep 0.2us, do 5 times to stay in this situation for 1us
    {
        ft2232h->append_data('\x80');  // GPIO command for ADBUS
        ft2232h->append_data('\x10');  // set CS high, MOSI and SCL low
        ft2232h->append_data(DIR_SPI);
    }
}
//--------------------------------------------------------------------------------
//this routine is used to disable SPI device
void MCP4922::SPI_CSDisable(void)
{
    for(int loop=0; loop<5; loop++) // one 0x80 command can keep 0.2us, do 5 times to stay in this situation for 1us
    {
        ft2232h->append_data('\x80');  // GPIO command for ADBUS
        ft2232h->append_data('\x00');  // set CS, MOSI and SCL low
        ft2232h->append_data(DIR_SPI);
    }
}
//--------------------------------------------------------------------------------
//this routine is used to initial SPI interface
BOOL MCP4922::SPI_Initial(DWORD dwClockDivisor)
{
    DWORD dwCount = 0;
    DWORD dwNumInputBuffer = 0;
    DWORD dwNumBytesRead = 0;
    FT_STATUS ftStatus = FT_OK;
    BYTE buf[1024];

    ftStatus = ft2232h->resetDevice();                          // Reset USB device
    // Purge USB receive buffer first by reading out all old data from FT2232H receive buffer
    ftStatus |= ft2232h->getQueueStatus(&dwNumInputBuffer);     // Get the number of bytes in the FT2232H receive buffer
    if ((ftStatus == FT_OK) && (dwNumInputBuffer > 0))
        ftStatus |= ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead); // Read out the data from FT2232H receive buffer
    ftStatus |= ft2232h->setUSBParameters(65535, 65535);        // Set USB request transfer size
    ftStatus |= ft2232h->setChars(false, 0, false, 0);          // Disable event and error characters
    ftStatus |= ft2232h->setTimeouts(3000, 3000);               // Sets the read and write timeouts in 3 sec for the FT2232H
    ftStatus |= ft2232h->setLatencyTimer(1);                    // Set the latency timer
    ftStatus |= ft2232h->setBitMode(0x0, 0x00);                 // Reset controller
    ftStatus |= ft2232h->setBitMode(0x0, 0x02);                 // Enable MPSSE mode
    if (ftStatus != FT_OK)
    {
        ft2232h->print_error("SPI_Initial", ftStatus);
        return FALSE;
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
        return FALSE; /* Error, can*t receive echo command , fail to synchronize MPSSE interface; */
    }
    //////////////////////////////////////////////////////////////////
    // Synchronize the MPSSE interface by sending bad command &xAB*
    //////////////////////////////////////////////////////////////////
    ft2232h->clear_data();              // Clear output buffer
    ft2232h->append_data('\xAB');       // Send BAD command &xAB*
    ftStatus = ft2232h->write_data();   // Send off the BAD commands

    ft2232h->clear_data();              // Clear output buffer
    do{
        ftStatus = ft2232h->getQueueStatus(&dwNumInputBuffer);  // Get the number of bytes in the device input buffer
    }while ((dwNumInputBuffer == 0) && (ftStatus == FT_OK));        // or Timeout

    bCommandEchod = false;
    ftStatus = ft2232h->read(&buf[0], dwNumInputBuffer, &dwNumBytesRead);   // Read out the data from input buffer
    for (dwCount = 0; dwCount < (dwNumBytesRead - 1); dwCount++)                    // Check if Bad command and echo command received
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
        return FALSE; /*Error, can't receive echo command , fail to synchronize MPSSE interface;*/
    }
    ////////////////////////////////////////////////////////////////////
    //Configure the MPSSE for SPI communication with EEPROM
    //////////////////////////////////////////////////////////////////
    ft2232h->append_data('\x8A');  // Ensure disable clock divide by 5 for 60Mhz master clock
    ft2232h->append_data('\x97');  // Ensure turn off adaptive clocking
    ft2232h->append_data('\x8D');  // disable 3 phase data clock
    ftStatus = ft2232h->write_data(); // Send out the commands

    ft2232h->clear_data();          // Clear output buffer
    ft2232h->append_data('\x80');   // Command to set directions of lower 8 pins and force value on bits set as output
    ft2232h->append_data('\x00');   // Set SDA, SCL high, WP disabled by SK, DO at bit &*, GPIOL0 at bit &*
    ft2232h->append_data(DIR_SPI);  // Set SK,DO,GPIOL0 pins as output with bit **, other pins as input with bit &*
    // The SK clock frequency can be worked out by below algorithm with divide by 5 set as off
    // SK frequency = 60MHz /((1 + [(1 +0xValueH*256) OR 0xValueL])*2)
    ft2232h->append_data('\x86');   // Command to set clock divisor
    ft2232h->append_data(BYTE(dwClockDivisor & '\xFF'));   // Set 0xValueL of clock divisor
    ft2232h->append_data(BYTE(dwClockDivisor >> 8));       // Set 0xValueH of clock divisor
    ft2232h->write_data();
    Sleeper::msleep(20);    // Delay for a while

    //Turn off loop back in case
    ft2232h->clear_data();          // Clear output buffer
    ft2232h->append_data('\x85');   // Command to turn off loop back of TDI/TDO connection
    ft2232h->write_data();

    ft2232h->clear_data();          // Clear output buffer
    Sleeper::msleep(30);            // Delay for a while

    emit info("SPI initial successful");
    return TRUE;
}
//--------------------------------------------------------------------------------
BOOL MCP4922::open(int deviceNumber)
{
    return ft2232h->open(deviceNumber);
}
//--------------------------------------------------------------------------------
BOOL MCP4922::close(void)
{
    return ft2232h->close();
}
//--------------------------------------------------------------------------------
/*
    A/B: DACA or DACB Select bit
    1 = Write to DACB
    0 = Write to DACA

    BUF: VREF Input Buffer Control bit
    1 = Buffered
    0 = Unbuffered

    GA: Output Gain Select bit
    1 = 1x (VOUT = VREF * D/4096)
    0 = 2x (VOUT = 2 * VREF * D/4096)

    SHDN: Output Power Down Control bit
    1 = Output Power Down Control bit
    0 = Output buffer disabled, Output is high impedance

    D11:D0: DAC Data bits
    12 bit number “D” which sets the output value. Contains a value between 0 and 4095.
*/
FT_STATUS MCP4922::set_voltage(float value)
{
    if(value > 3.132f)
    {
        emit error(QString("value %1 too big (>3.132)").arg(value));
        return FT_OTHER_ERROR;
    }

    MCP4922_DATA data;
    // data.bits.DATA = ((float)value * (float)3132.0f) / (float)4096.0f + 0.5f; // max 4095
    data.bits.DATA = ((float)value * (float)4096.0f) / (float)3.132f + 0.5f; // max 4095
    data.bits.SHDN = 1;
    data.bits.GA   = 1;
    data.bits.BUF  = 0;
    data.bits.AB   = 0;

    ft2232h->clear_data();
    SPI_CSDisable();

    emit info(QString("set %1 volt").arg(value));
    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7); // 7+1 = 8
    ft2232h->append_data(BYTE(data.value >> 8));       // Set 0xValueH of clock divisor

    ft2232h->append_data(MSB_FALLING_EDGE_CLOCK_BIT_OUT);
    ft2232h->append_data(7); // 7+1 = 8
    ft2232h->append_data(BYTE(data.value & '\xFF'));   // Set 0xValueL of clock divisor

    SPI_CSEnable();

    return ft2232h->write_data();
}
//--------------------------------------------------------------------------------
void MCP4922::updateText(void)
{

}
//--------------------------------------------------------------------------------
