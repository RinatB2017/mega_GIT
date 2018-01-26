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
#include <ftd2xx.h>
#include <ftdi.h>
//--------------------------------------------------------------------------------
#include "sleeper.h"
#include "hd44780.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
/*
http://easyelectronics.ru/avr-uchebnyj-kurs-podklyuchenie-k-avr-lcd-displeya-hd44780.html
Типовой 14ти пиновый интерфейс дисплеев на HD44780:

    1.  Земля, общий провод, GND
    2.  Напряжение питания, Vcc (+5V)
    3.  Настройка контрастности (Vo) - 0
    4.  Выбор регистра (R/S для HD44780, A0 для KS0066) - определяет что у нас передается, команда (RS=0) или данные (RS=1).
                                                          Данные будут записаны в память по текущему адресу, а команда исполнена контроллером.
    5.  Чтение/запись (R/W) - 1/0
    6.  Строб по спаду (Enable)
    7.  Bit 0 (младший для 8-битного интерфейса)
    8.  Bit 1
    9.  Bit 2
    10. Bit 3
    11. Bit 4 (младший для 4-битного интерфейса)
    12. Bit 5
    13. Bit 6
    14. Bit 7 (старший для 8ми (4х) битного интерфейса)
    15. Питание подсветки для дисплеев с подсветкой (анод)
    16. Питание подсветки для дисплеев с подсветкой (катод)

Дисплей может работать в 4-х или 8-ми битном режимах.
В первом случае пины с седьмого по десятый не используются,
а данные передаются через 11 — 14 пины, по четыре бита за такт
(старший полу-байт, затем младший полу-байт).

DB7	DB6	DB5	DB4	DB3	DB2	DB1	DB0	Значение
0	0	0	0	0	0	0	1	Очистка экрана. Счетчик адреса на 0 позицию DDRAM
0	0	0	0	0	0	1	-	Адресация на DDRAM сброс сдвигов, Счетчик адреса на 0
0	0	0	0	0	1	I/D	S	Настройка сдвига экрана и курсора
0	0	0	0	1	D	C	B	Настройка режима отображения
0	0	0	1	S/C	R/L	-	-	Сдвиг курсора или экрана, в зависимости от битов
0	0	1	DL	N	F	-	-	Выбор числа линий, ширины шины и размера символа
0	1	AG	AG	AG	AG	AG	AG	Переключить адресацию на SGRAM и задать адрес в SGRAM
1	AD	AD	AD	AD	AD	AD	AD	Переключить адресацию на DDRAM и задать адрес в DDRAM

I/D — инкремент или декремент счетчика адреса. По дефолту стоит 0 — Декремент. Т.е. каждый следующий байт будет записан в n-1 ячейку. Если поставить 1 — будет Инкремент.
S - сдвиг экрана, если поставить 1 то с каждым новым символом будет сдвигаться окно экрана, пока не достигнет конца DDRAM, наверное удобно будет когда выводишь на экран здоровенную строку, на все 40 символов, чтобы не убегала за экран.
D — включить дисплей. Если поставить туда 0 то изображение исчезнет, а мы в это время можем в видеопамяти творить всякие непотребства и они не будут мозолить глаза. А чтобы картинка появилась в эту позицию надо записать 1.
С - включить курсор в виде прочерка. Все просто, записали сюда 1 — включился курсор.
B — сделать курсор в виде мигающего черного квадрата.
S/C сдвиг курсора или экрана. Если стоит 0, то сдвигается курсор. Если 1, то экран. По одному разу за команду
R/L — определяет направление сдвига курсора и экрана. 0 — влево, 1 — вправо.
D/L — бит определяющий ширину шины данных. 1-8 бит, 0-4 бита
N — число строк. 0 — одна строка, 1 — две строки.
F - размер символа 0 — 5х8 точек. 1 — 5х10 точек (встречается крайне редко)
AG - адрес в памяти CGRAM
АD — адрес в памяти DDRAM
*/
//--------------------------------------------------------------------------------
union B_BYTE
{
    unsigned char value;
    struct
    {
        unsigned bit0:1;
        unsigned bit1:1;
        unsigned bit2:1;
        unsigned bit3:1;
        unsigned bit4:1;
        unsigned bit5:1;
        unsigned bit6:1;
        unsigned bit7:1;
    } bites;
};

union D_BYTE
{
    unsigned char value;
    struct
    {
        unsigned a:4;
        unsigned b:4;
    } decades;
};
//--------------------------------------------------------------------------------
HD44780::HD44780(QWidget *parent) :
    MyWidget(parent)
{

}
//--------------------------------------------------------------------------------
HD44780::~HD44780()
{
    close();
}
//--------------------------------------------------------------------------------
void HD44780::test(void)
{

}
//--------------------------------------------------------------------------------
bool HD44780::open(void)
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
    ftStatus = FT_ListDevices((PVOID)devIndex ,&Buf, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);

    ftStatus = FT_Open(0, &ftHandle);
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
bool HD44780::VerifyMPSSE(void)
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
void HD44780::ConfigureMPSSE(void)
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
void HD44780::close(void)
{
    ftStatus = FT_Close(ftHandle);
    if(ftStatus == FT_OK)
        ftHandle = 0;
}
//--------------------------------------------------------------------------------
void HD44780::screen_on(void)
{
    dwNumBytesToSend = 0;   //Clear output buffer

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the commands
    if(ftStatus != FT_OK)
    {
        emit error(QString("screen_on: FT_Write(3) return %1").arg(ftStatus));
    }
}
//--------------------------------------------------------------------------------
void HD44780::screen_off(void)
{
    dwNumBytesToSend = 0;   //Clear output buffer

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the commands
    if(ftStatus != FT_OK)
    {
        emit error(QString("screen_off: FT_Write(3) return %1").arg(ftStatus));
    }
}
//--------------------------------------------------------------------------------
void HD44780::clear(void)
{
    dwNumBytesToSend = 0;   //Clear output buffer

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the commands
    if(ftStatus != FT_OK)
    {
        emit error(QString("clear: FT_Write(3) return %1").arg(ftStatus));
    }
}
//--------------------------------------------------------------------------------
void HD44780::move_cursor(void)
{
    dwNumBytesToSend = 0;   //Clear output buffer

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the commands
    if(ftStatus != FT_OK)
    {
        emit error(QString("move_cursor: FT_Write(3) return %1").arg(ftStatus));
    }
}
//--------------------------------------------------------------------------------
void HD44780::write(unsigned char data)
{
    dwNumBytesToSend = 0;   //Clear output buffer

    OutputBuffer[dwNumBytesToSend++] = data;

    ftStatus = FT_Write(ftHandle, OutputBuffer, dwNumBytesToSend, &dwNumBytesSent);	// Send off the commands
    if(ftStatus != FT_OK)
    {
        emit error(QString("write: FT_Write(3) return %1").arg(ftStatus));
    }
}
//--------------------------------------------------------------------------------
void HD44780::strobe(void)
{

}
//--------------------------------------------------------------------------------
void HD44780::print_error(const QString &function,
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
void HD44780::updateText(void)
{

}
//--------------------------------------------------------------------------------
