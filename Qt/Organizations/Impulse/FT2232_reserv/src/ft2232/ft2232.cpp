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
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QFrame>

#include "doublehexeditor.hpp"
#include "ft2232.hpp"
#include "sleeper.h"
//--------------------------------------------------------------------------------
#define SDA 2
#define SCL 1
//--------------------------------------------------------------------------------
union UCHAR8
{
    unsigned char value;
    struct
    {
        unsigned bit_SCL:1;
        unsigned bit_SDA:1;
        unsigned reserved:6;
    } bits;
};
//--------------------------------------------------------------------------------
FT2232::FT2232(QWidget *parent) :
    QWidget(parent),
    BaudRate(4800)
{
    connect_log();
}
//--------------------------------------------------------------------------------
void FT2232::connect_log(void)
{
    if(parentWidget())
    {
        // qDebug() << "parent is true";
        connect(this, SIGNAL(info(QString)),  parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)), parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)), parentWidget(), SIGNAL(error(QString)));
    }
    else
    {
        // qDebug() << "parent is false";
        connect(this, SIGNAL(info(QString)),  this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)), this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)), this, SLOT(log(QString)));
    }
}
//--------------------------------------------------------------------------------
void FT2232::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
void FT2232::SetBaudRate(ULONG BaudRate)
{
    this->BaudRate = BaudRate;
}
//--------------------------------------------------------------------------------
bool FT2232::Open(void)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD devIndex = 0;
    char Buf[64];

    ftStatus = FT_ListDevices((PVOID)devIndex, &Buf, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
    if(ftStatus == FT_OK)
    {
        //emit info(QString("Buf [%1]").arg(QByteArray(Buf).toHex().data()));
        emit info(QString("devIndex %1 Buf [%2]")
                  .arg(devIndex)
                  .arg(Buf));
    }
    ftStatus = FT_Open(0, &ftHandle);

    if(ftStatus != FT_OK)
    {
        emit error("Can't open FT2232H device!");
        return false;
    }

    ftStatus |= FT_SetBitMode(ftHandle, 0x0, 0x00);             // Reset
    ftStatus |= FT_SetBitMode(ftHandle, 0x3, 0x04);             // Synchronous Bit Bang (FT232R, FT245R, FT2232D, FT2232H, FT4232H and FT232H devices only)
    ftStatus |= FT_SetBaudRate(ftHandle, BaudRate);             // freq = x*16

    // Inform the user if any errors were encountered
    if(ftStatus != FT_OK)
    {
        emit error("failure to initialize FT232H device!");
        return false;
    }

    emit info("Successfully open FT2232H device!");
    return true;
}
//--------------------------------------------------------------------------------
FT_STATUS FT2232::Close(void)
{
    return FT_Close(ftHandle);
}
//--------------------------------------------------------------------------------
#if 0
void i2c_start (void)
{
    SDA_HI(); // SDATA = HIGH;							// Set data line high
    SCL_HI(); // SCLK = HIGH;							// Set clock line high
    SDA_LO(); // SDATA = LOW;							// Set data line low (START SIGNAL)
    SCL_LO(); // SCLK = LOW;							// Set clock line low
}
#endif
//--------------------------------------------------------------------------------
void FT2232::send_byte(unsigned char data)
{
    unsigned char output_data = data;
    dwNumBytesToSend = 0;                           // Clear output buffer˰
    // QString temp;

    // temp.clear();
    for(int index=0; index < 8; index++)
    {
        //OutputBuffer[dwNumBytesToSend++] = 0x82;    // Set Data Bits Low Byte
        OutputBuffer[dwNumBytesToSend++] = (output_data & 0x80) ? (SDA | SCL) : SCL;    // value
        //OutputBuffer[dwNumBytesToSend++] = (output_data & 0x80) ? (SDA | SCL) : SCL;    // value
        // OutputBuffer[dwNumBytesToSend++] = 0x13;    // direction

        // temp.append((output_data & 0x80) ? "1" : "0");
        output_data <<= 1;
    }
    //OutputBuffer[dwNumBytesToSend++] = 0x83;
    //OutputBuffer[dwNumBytesToSend++] = 0x87;

    // qDebug() << temp;
}
//--------------------------------------------------------------------------------
bool FT2232::test(void)
{
    FT_STATUS ftStatus = FT_OK;

    //    dwNumBytesToSend = 0;   // Clear output buffer˰
    //    OutputBuffer[dwNumBytesToSend++] = SDA | SCL;
    //    write_OutputBuffer();

    bool ok = SetStart();
    if(!ok)
    {
        emit error("SetStart is FALSE");
        return false;
    }

    ok = send_byte_and_check_ACK(160);
    if(!ok)
    {
        emit error("send 160 is false");
        return false;
    }
    ok = send_byte_and_check_ACK(0);
    if(!ok)
    {
        emit error("send 0 is false");
        return false;
    }
    ok = send_byte_and_check_ACK(10);
    if(!ok)
    {
        emit error("send 0 is false");
        return false;
    }

    ok = SetStart();
    if(!ok)
    {
        emit error("SetStart is FALSE");
        return false;
    }

    ok = send_byte_and_check_ACK(161);
    if(!ok)
    {
        emit error("send 160 is false");
        return false;
    }

    //---
    //    dwNumBytesToRead = 0;
    //    OutputBuffer[dwNumBytesToSend++] = 0x83;
    //    write_OutputBuffer();

    ftStatus = FT_GetQueueStatus(ftHandle, &dwNumBytesToRead);      // Get number of bytes in the input buffer
    if(ftStatus == FT_OK)
    {
        ftStatus = FT_Read(ftHandle, &InputBuffer, dwNumBytesToRead, &dwNumBytesRead);
        if(ftStatus == FT_OK)
        {
            emit debug(QString("in the input buffer %1 bytes").arg(dwNumBytesRead));
            if(dwNumBytesRead)
            {
                QByteArray ba;
                ba.clear();
                for(DWORD n=0; n<dwNumBytesRead; n++)
                    ba.append(InputBuffer[n]);
                emit debug(QString("data [%1]").arg(ba.toHex().data()));
            }
        }
    }

    //---

    ok = SetStop();
    if(!ok)
    {
        emit error("SetStop is FALSE");
        return false;
    }

    emit info("Test is OK!!!");
    return true;
}
//--------------------------------------------------------------------------------
bool FT2232::send_byte_and_check_ACK(unsigned char data)
{
    send_byte(data);
    if(!write_OutputBuffer())
    {
        emit error("send_byte_and_check_ACK: write_OutputBuffer is FALSE");
        return false;
    }

    bool ok = check_ACK();
    if(!ok)
    {
        emit error("send_byte_and_check_ACK: check_ACK is FALSE");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool FT2232::SetStart(void)
{
    emit info("SetStart: begin");
    dwNumBytesToSend = 0;                           // Clear output buffer˰

#if 1
    OutputBuffer[dwNumBytesToSend++] = SCL | SDA;
    OutputBuffer[dwNumBytesToSend++] = SCL;
    OutputBuffer[dwNumBytesToSend++] = 0;
#else
    UCHAR8 data;
    data.value = 0;

    data.bits.bit_SCL = 1;
    data.bits.bit_SDA = 1;
    OutputBuffer[dwNumBytesToSend++] = 0x82;
    OutputBuffer[dwNumBytesToSend++] = data.value;
    OutputBuffer[dwNumBytesToSend++] = 0x03;

    data.bits.bit_SCL = 1;
    data.bits.bit_SDA = 0;
    OutputBuffer[dwNumBytesToSend++] = 0x82;
    OutputBuffer[dwNumBytesToSend++] = data.value;
    OutputBuffer[dwNumBytesToSend++] = 0x03;

    data.bits.bit_SCL = 0;
    data.bits.bit_SDA = 0;
    OutputBuffer[dwNumBytesToSend++] = 0x82;
    OutputBuffer[dwNumBytesToSend++] = data.value;
    OutputBuffer[dwNumBytesToSend++] = 0x03;
#endif

    emit info("SetStart: write_OutputBuffer");
    if(!write_OutputBuffer())
        return false;

    emit info("SetStart: check_ACK");
    if(!check_ACK())
        return false;

    emit info("SetStart: end");
    return true;
}
//--------------------------------------------------------------------------------
#if 0
void i2c_stop (void)
{
    unsigned char input_var;

    SCL_LO(); // SCLK = LOW;								// Set clock line low
    SDA_LO(); // SDATA = LOW;							// Set data line low
    SCL_HI(); // SCLK = HIGH;							// Set clock line high
    SDA_HI(); //SDATA = HIGH;							// Set data line high (STOP SIGNAL)
    input_var = GPIO_ReadInputData(GPIOA);						// Put port pin into HiZ
}
#endif
//--------------------------------------------------------------------------------
bool FT2232::SetStop(void)
{
    //FT_STATUS ftStatus = FT_OK;
    dwNumBytesToSend = 0;                                   // Clear output buffer˰

    //OutputBuffer[dwNumBytesToSend++] = 0x82;        // Set Data Bits Low Byte
    //OutputBuffer[dwNumBytesToSend++] = 0;           // value
    //OutputBuffer[dwNumBytesToSend++] = 0x3;         // direction

    //OutputBuffer[dwNumBytesToSend++] = 0x82;        // Set Data Bits Low Byte
    //OutputBuffer[dwNumBytesToSend++] = SDA | SCL;   // value
    //OutputBuffer[dwNumBytesToSend++] = 0x3;         // direction

    OutputBuffer[dwNumBytesToSend++] = 0;           // value
    OutputBuffer[dwNumBytesToSend++] = SDA | SCL;   // value

    return write_OutputBuffer();
}
//--------------------------------------------------------------------------------
#if 0
void i2c_write (unsigned char output_data)
{
    unsigned char index;

    for(index = 0; index < 8; index++)  	// Send 8 bits to the I2C Bus
    {
        // Output the data bit to the I2C Bus
        (output_data & 0x80) ? SDA_HI() : SDA_LO(); //SDATA = ((output_data & 0x80) ? 1 : 0);
        output_data  <<= 1;            		// Shift the byte by one bit
        SCL_HI();   //SCLK = HIGH;   		        		// Clock the data into the I2C Bus
        SCL_LO(); //SCLK = LOW;
    }

    //index = SDATA;							// Put data pin into read mode
    SCL_HI();   // SCLK = HIGH;	// Clock the ACK from the I2C Bus
    SCL_LO(); // SCLK = LOW;
}
#endif
//--------------------------------------------------------------------------------
bool FT2232::SendByteAndCheckACK(unsigned char data)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD ReadTimeoutCounter = 0;

    dwNumBytesToSend = 0;   // Clear output buffer
    send_byte(data);
    //OutputBuffer[dwNumBytesToSend++] = 0x83;

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands
    dwNumBytesToRead = 0;
    ReadTimeoutCounter = 0;
    ftStatus = FT_GetQueueStatus(ftHandle, &dwNumBytesToRead);      // Get number of bytes in the input buffer
    while ((dwNumBytesToRead < 1) && (ftStatus == FT_OK) && (ReadTimeoutCounter < 500))
    {
        ftStatus = FT_GetQueueStatus(ftHandle, &dwNumBytesToRead);  // Get # bytes in buffer
        ReadTimeoutCounter ++;
        Sleeper::msleep(1);   // short delay
    }
    if ((ftStatus == FT_OK) && (ReadTimeoutCounter < 500))
    {
        ftStatus = FT_Read(ftHandle, &InputBuffer, dwNumBytesToRead, &dwNumBytesRead);
        if (((InputBuffer[0] & 0x01) == 0x00))  // Check ACK bit 0 on data byte read out
        {
            return true; // Return True if the ACK was received
        }
        else
        {
            emit error(QString("read %1 bytes [0]=%2")
                       .arg(dwNumBytesRead)
                       .arg(InputBuffer[0]));
            emit error("SendByteAndCheckACK: Failed to get ACK from I2C Slave");

            QByteArray ba_i;
            QByteArray ba_o;
            ba_i.clear();
            ba_o.clear();

            for(DWORD n=0; n<dwNumBytesRead; n++) ba_i.append(InputBuffer[n]);
            for(DWORD n=0; n<dwNumBytesSent; n++) ba_o.append(OutputBuffer[n]);

            DoubleHexEditor *frame = new DoubleHexEditor("output", "input");
            frame->append(ba_o, ba_i);
            frame->show();
            return false;
        }
    }
    else
    {
        emit error("SendByteAndCheckACK: Failed to get any data back or got an error code back");
        Print_FT_STATUS(ftStatus);
        return false;
    }
}
//--------------------------------------------------------------------------------
#if 0
function Send_Byte_Then_Ignore_ACK(Data : byte) : boolean;
var SDA,passed : boolean;
begin
Send_Byte_Then_Ignore_ACK := true;
Out_Buff[0] := Data;
passed := ScanOut(8,@Out_Buff);
if passed then
begin
AddToBuffer($80);
AddToBuffer(Saved_Port_Value);
AddToBuffer($11); // set SCL,WP as out SDA as in
Out_Buff[0] := 0;
passed := ScanOut(1,@Out_Buff);
AddToBuffer($80);
AddToBuffer(Saved_Port_Value);
AddToBuffer($13); // set SCL,SDA,WP as out
end;
end;
#endif
//--------------------------------------------------------------------------------
bool FT2232::SendByteAndIgnoreACK(unsigned char data)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD ReadTimeoutCounter = 0;

    dwNumBytesToSend = 0;                       // Clear output buffer
    send_byte(data);

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent); //Send off the commands
    dwNumBytesToRead = 0;
    ReadTimeoutCounter = 0;
    ftStatus = FT_GetQueueStatus(ftHandle, &dwNumBytesToRead); // Get number of bytes in the input buffer
    while ((dwNumBytesToRead < 1) && (ftStatus == FT_OK) && (ReadTimeoutCounter < 500))
    {
        ftStatus = FT_GetQueueStatus(ftHandle, &dwNumBytesToRead); // Get # bytes in buffer
        ReadTimeoutCounter ++;
        Sleeper::msleep(1);   // short delay
    }
    if ((ftStatus == FT_OK) && (ReadTimeoutCounter < 500))
    {
        ftStatus = FT_Read(ftHandle, &InputBuffer, dwNumBytesToRead, &dwNumBytesRead);
        return true; // Return True if the ACK was received
    }
    else
    {
        emit error("SendByteAndCheckACK: Failed to get any data back or got an error code back");
        Print_FT_STATUS(ftStatus);
        return false;
    }
}
//--------------------------------------------------------------------------------
#if 0
function Read_Byte_Then_Chk_ACK(var Data : byte) : boolean;
var SDA,passed : boolean;
i : integer;
begin
Read_Byte_Then_Chk_ACK := false;
AddToBuffer($80);
AddToBuffer(Saved_Port_Value);
AddToBuffer($11); // set SCL,WP as out SDA as in
passed := ScanIn(9);
AddToBuffer($87);  //Send immediate
SendBytes(OutIndex); // send off the command
Read_Data(@In_Buff,9);
Data := In_Buff[0];
i := In_Buff[1] AND $01;
if (i = 0 ) then
Read_Byte_Then_Chk_ACK := true;
AddToBuffer($80);
AddToBuffer(Saved_Port_Value);
AddToBuffer($13); // set SCL,SDA,WP as out
end;
#endif
//--------------------------------------------------------------------------------
bool FT2232::ReadByteAndCheckACK(unsigned char *data)
{
    return true;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
#if 0
function Init_Controller(DName : String) : boolean;
var passed : boolean;
res : FT_Result;
begin
Init_Controller := false;
passed := OpenPort(DName);
if passed then
begin
res := Set_USB_Device_LatencyTimer(16);
res := Set_USB_Device_BitMode($00,$00); // reset controller
res := Set_USB_Device_BitMode($00,$02); // enable JTAG controller
passed := Sync_To_MPSSE;
end;
if passed then
begin
OutIndex := 0;

speed = 60
        //  sleep(20); // wait for all the USB stuff to complete
        AddToBuffer($80); // set SK,DO,CS as out
AddToBuffer($13); // SDA SCL WP high
Saved_Port_Value := $13;
AddToBuffer($13); // inputs on GPIO12-14
AddToBuffer($82); // outputs on GPIO21-24
AddToBuffer($0F);
AddToBuffer($0F);
AddToBuffer($86); // set clk divisor
AddToBuffer(speed AND $FF);
AddToBuffer(speed SHR 8);
// turn off loop back
AddToBuffer($85);
SendBytes(OutIndex); // send off the command
Init_Controller := true;
end;
end;
#endif
//--------------------------------------------------------------------------------
bool FT2232::Init(void)
{
#if 0
    unsigned int speed = 60;

    FT_STATUS ftStatus = FT_OK;
    dwNumBytesToSend = 0;                           // Clear output buffer˰

    OutputBuffer[dwNumBytesToSend++] = 0x80;
    OutputBuffer[dwNumBytesToSend++] = 0x13;
    OutputBuffer[dwNumBytesToSend++] = 0x13;
    OutputBuffer[dwNumBytesToSend++] = 0x82;
    OutputBuffer[dwNumBytesToSend++] = 0x0F;
    OutputBuffer[dwNumBytesToSend++] = 0x0F;
    OutputBuffer[dwNumBytesToSend++] = 0x86;
    OutputBuffer[dwNumBytesToSend++] = (speed & 0xFF);
    OutputBuffer[dwNumBytesToSend++] = (speed >> 8);

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        emit error("SetI2CStart: FT_Write is failed!");
        Print_FT_STATUS(ftStatus);
        return false;
    }
#endif
    return true;
}
//--------------------------------------------------------------------------------
bool FT2232::write_OutputBuffer(void)
{
    FT_STATUS ftStatus = FT_OK;

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);
    if(ftStatus != FT_OK)
    {
        emit error("write_OutputBuffer: FT_Write is failed!");
        Print_FT_STATUS(ftStatus);
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
FT_STATUS FT2232::clear_buffer(void)
{
    FT_STATUS ftStatus = FT_OK;

    ftStatus = FT_GetQueueStatus(ftHandle, &dwNumBytesToRead);
    if(ftStatus == FT_OK)
    {
        if(dwNumBytesToRead)
        {
            ftStatus = FT_Read(ftHandle, &InputBuffer, dwNumBytesToRead, &dwNumBytesRead);
        }
    }
    return ftStatus;
}
//--------------------------------------------------------------------------------
bool FT2232::check_ACK(void)
{
    FT_STATUS ftStatus = FT_OK;
    int cnt = 0;

    QCoreApplication::processEvents();
    clear_buffer();

    dwNumBytesToRead = 0;
    OutputBuffer[dwNumBytesToSend++] = 0x83;

    if(!write_OutputBuffer())
        return false;

    while(cnt < 1000)
    {
        ftStatus = FT_GetQueueStatus(ftHandle, &dwNumBytesToRead);
        if(ftStatus == FT_OK)
        {
            if(dwNumBytesToRead)
            {
                emit debug(QString("check_ACK: dwNumBytesToRead = %1").arg(dwNumBytesToRead));
                ftStatus = FT_Read(ftHandle, &InputBuffer, dwNumBytesToRead, &dwNumBytesRead);
                if(ftStatus == FT_OK)
                {
                    if(dwNumBytesRead)
                    {
                        emit debug(QString("read %1 bytes").arg(dwNumBytesRead));

                        QByteArray ba;
                        ba.clear();
                        for(DWORD n=0; n<dwNumBytesRead; n++)
                            ba.append(InputBuffer[n]);
                        emit debug(ba.toHex().toUpper());

                        UCHAR8 data;
                        data.value = InputBuffer[dwNumBytesToRead];
                        emit debug(QString("value %1").arg(data.value));
                        emit debug(QString("SDA %1").arg(data.bits.bit_SDA));
                        emit debug(QString("SCL %1").arg(data.bits.bit_SCL));

                        if((data.bits.bit_SCL == 1) && (data.bits.bit_SDA == 0))
                            return true;
                    }
                }
                break;
            }
        }
        cnt++;
        Sleeper::msleep(1);
    }
    return false;
}
//--------------------------------------------------------------------------------
void FT2232::Print_FT_STATUS(FT_STATUS status)
{
    switch(status)
    {
    case FT_OK:
        break;

    case FT_INVALID_HANDLE:
        emit error("Error: FT_INVALID_HANDLE");
        break;
    case FT_DEVICE_NOT_FOUND:
        emit error("Error: FT_DEVICE_NOT_FOUND");
        break;
    case FT_DEVICE_NOT_OPENED:
        emit error("Error: FT_DEVICE_NOT_OPENED");
        break;
    case FT_IO_ERROR:
        emit error("Error: FT_IO_ERROR");
        break;
    case FT_INSUFFICIENT_RESOURCES:
        emit error("Error: FT_INSUFFICIENT_RESOURCES");
        break;
    case FT_INVALID_PARAMETER:
        emit error("Error: FT_INVALID_PARAMETER");
        break;
    case FT_INVALID_BAUD_RATE:
        emit error("Error: FT_INVALID_BAUD_RATE");
        break;
    case FT_DEVICE_NOT_OPENED_FOR_ERASE:
        emit error("Error: FT_DEVICE_NOT_OPENED_FOR_ERASE");
        break;
    case FT_DEVICE_NOT_OPENED_FOR_WRITE:
        emit error("Error: FT_DEVICE_NOT_OPENED_FOR_WRITE");
        break;
    case FT_FAILED_TO_WRITE_DEVICE:
        emit error("Error: FT_FAILED_TO_WRITE_DEVICE");
        break;
    case FT_EEPROM_READ_FAILED:
        emit error("Error: FT_EEPROM_READ_FAILED");
        break;
    case FT_EEPROM_WRITE_FAILED:
        emit error("Error: FT_EEPROM_WRITE_FAILED");
        break;
    case FT_EEPROM_ERASE_FAILED:
        emit error("Error: FT_EEPROM_ERASE_FAILED");
        break;
    case FT_EEPROM_NOT_PRESENT:
        emit error("Error: FT_EEPROM_NOT_PRESENT");
        break;
    case FT_EEPROM_NOT_PROGRAMMED:
        emit error("Error: FT_EEPROM_NOT_PROGRAMMED");
        break;
    case FT_INVALID_ARGS:
        emit error("Error: FT_INVALID_ARGS");
        break;
    case FT_NOT_SUPPORTED:
        emit error("Error: FT_NOT_SUPPORTED");
        break;
    case FT_OTHER_ERROR:
        emit error("Error: FT_OTHER_ERROR");
        break;

    default:
        emit error(QString("Error: unknown code %1").arg(status));
        break;
    }
}
//--------------------------------------------------------------------------------
