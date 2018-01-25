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
#include <mpsse.h>

#include "sleeper.h"
#include "test_mpsse.hpp"
//--------------------------------------------------------------------------------
#define SIZE	0x8000          // Size of EEPROM chip (32KB)
#define WCMD	"\xA0\x00\x00"	// Write start address command
#define RCMD	"\xA1"          // Read command
#define FOUT	"eeprom.bin"	// Output file
//--------------------------------------------------------------------------------
Test_MPSSE::Test_MPSSE(QWidget *parent) :
    QWidget(parent)
{

}
//--------------------------------------------------------------------------------
void Test_MPSSE::connect_log(void)
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
void Test_MPSSE::log(const QString &data)
{
    qDebug() << data;
}
//--------------------------------------------------------------------------------
int Test_MPSSE::test_bitbang(void)
{
    struct mpsse_context *io = NULL;
    int i = 0, retval = EXIT_FAILURE;

    io = MPSSE(BITBANG, 0, 0);

    if(io && io->open)
    {
        for(i=0; i<10; i++)
        {
            PinHigh(io, 0);
            emit info(QString("Pin 0 is: %1")
                      .arg(PinState(io, 0, -1)));
            Sleeper::msleep(1);

            ::PinLow(io, 0);
            emit info(QString("Pin 0 is: %1")
                      .arg(PinState(io, 0, -1)));
            Sleeper::msleep(1);
        }

        retval = EXIT_SUCCESS;
    }
    else
    {
        emit error(QString("Failed to open MPSSE: %1")
                   .arg(ErrorString(io)));
    }

    Close(io);

    return retval;
}
//--------------------------------------------------------------------------------
int Test_MPSSE::test_ds1305(void)
{
    struct mpsse_context *ds1305 = NULL;
    int sec = 0, min = 0, retval = EXIT_FAILURE;
    char *control = NULL, *seconds = NULL, *minutes = NULL;

    if((ds1305 = MPSSE(SPI3, ONE_HUNDRED_KHZ, MSB)) != NULL && ds1305->open)
    {
        SetCSIdle(ds1305, 0);

        emit info(QString("%1 initialized at %2Hz (SPI mode 3)")
                  .arg(GetDescription(ds1305))
                       .arg(GetClock(ds1305)));

        Start(ds1305);
        Write(ds1305, "\x0F", 1);
        control = Read(ds1305, 1);
        Stop(ds1305);

        control[0] &= ~0x80;

        Start(ds1305);
        Write(ds1305, "\x8F", 1);
        Write(ds1305, control, 1);
        Stop(ds1305);

        free(control);

        while(1)
        {
            Sleeper::msleep(1);

            Start(ds1305);
            Write(ds1305, "\x00", 1);
            seconds = Read(ds1305, 1);
            Stop(ds1305);

            sec = (((seconds[0] >> 4) * 10) + (seconds[0] & 0x0F));

            Start(ds1305);
            Write(ds1305, "\x01", 1);
            minutes = Read(ds1305, 1);
            Stop(ds1305);

            min = (((minutes[0] >> 4) * 10) + (minutes[0] & 0x0F));

            emit info(QString("%.1:%.2")
                      .arg(min)
                      .arg(sec));

            free(minutes);
            free(seconds);
        }
    }
    else
    {
        emit error(QString("Failed to initialize MPSSE: %1")
                   .arg(ErrorString(ds1305)));
    }

    Close(ds1305);

    return retval;
}
//--------------------------------------------------------------------------------
int Test_MPSSE::test_i2ceeprom(void)
{
    FILE *fp = NULL;
    char *data = NULL;
    int retval = EXIT_FAILURE;
    struct mpsse_context *eeprom = NULL;

    if((eeprom = MPSSE(I2C, FOUR_HUNDRED_KHZ, MSB)) != NULL && eeprom->open)
    {
        emit info(QString("%1 initialized at %2Hz (I2C)")
                  .arg(GetDescription(eeprom))
                  .arg(GetClock(eeprom)));

        /* Write the EEPROM start address */
        Start(eeprom);
        Write(eeprom, WCMD, sizeof(WCMD) - 1);

        if(GetAck(eeprom) == ACK)
        {
            /* Send the EEPROM read command */
            Start(eeprom);
            Write(eeprom, RCMD, sizeof(RCMD) - 1);

            if(GetAck(eeprom) == ACK)
            {
                /* Read in SIZE bytes from the EEPROM chip */
                data = Read(eeprom, SIZE);
                if(data)
                {
                    fp = fopen(FOUT, "wb");
                    if(fp)
                    {
                        fwrite(data, 1, SIZE, fp);
                        fclose(fp);

                        emit info(QString("Dumped %1 bytes to %2")
                                  .arg(SIZE)
                                  .arg(FOUT));
                        retval = EXIT_SUCCESS;
                    }

                    free(data);
                }

                /* Tell libmpsse to send NACKs after reading data */
                SendNacks(eeprom);

                /* Read in one dummy byte, with a NACK */
                Read(eeprom, 1);
            }
        }

        Stop(eeprom);
    }
    else
    {
        emit error(QString("Failed to initialize MPSSE: %1")
                   .arg(ErrorString(eeprom)));
    }

    Close(eeprom);

    return retval;
}
//--------------------------------------------------------------------------------
int Test_MPSSE::test_gpio(void)
{
    struct mpsse_context *io = NULL;
    int i = 0, retval = EXIT_FAILURE;

    io = MPSSE(GPIO, 0, 0);

    if(io && io->open)
    {
        for(i=0; i<10; i++)
        {
            PinHigh(io, GPIOL0);
            emit info(QString("GPIOL0 State: %1")
                      .arg(PinState(io, GPIOL0, -1)));
            Sleeper::msleep(1);

            PinLow(io, GPIOL0);
            emit info(QString("GPIOL0 State: %1")
                      .arg(PinState(io, GPIOL0, -1)));
            Sleeper::msleep(1);
        }

        retval = EXIT_SUCCESS;
    }
    else
    {
        emit error(QString("Failed to open MPSSE: %1")
                   .arg(ErrorString(io)));
    }

    Close(io);

    return retval;
}
//--------------------------------------------------------------------------------
int Test_MPSSE::test_spiflash(void)
{
    FILE *fp = NULL;
    char *data = NULL, *data1 = NULL;
    int retval = EXIT_FAILURE;
    struct mpsse_context *flash = NULL;

    if((flash = MPSSE(SPI0, TWELVE_MHZ, MSB)) != NULL && flash->open)
    {
        emit info(QString("%1 initialized at %2Hz (SPI mode 0)")
                  .arg(GetDescription(flash))
                  .arg(GetClock(flash)));

        Start(flash);
        Write(flash, RCMD, sizeof(RCMD) - 1);
        data = Read(flash, SIZE);
        data1 = Read(flash, SIZE);
        Stop(flash);

        if(data)
        {
            fp = fopen(FOUT, "wb");
            if(fp)
            {
                fwrite(data, 1, SIZE, fp);
                fwrite(data1, 1, SIZE, fp);
                fclose(fp);

                emit info(QString("Dumped %1 bytes to %2")
                          .arg(SIZE)
                          .arg(FOUT));
                retval = EXIT_SUCCESS;
            }

            free(data);
        }
    }
    else
    {
        emit error(QString("Failed to initialize MPSSE: %1")
                   .arg(ErrorString(flash)));
    }

    Close(flash);

    return retval;
}
//--------------------------------------------------------------------------------
int Test_MPSSE::test_spiflashfast(void)
{
    FILE *fp = NULL;
    char data[SIZE] = { 0 };
    int retval = EXIT_FAILURE;
    struct mpsse_context *flash = NULL;

    if((flash = MPSSE(SPI0, TWELVE_MHZ, MSB)) != NULL && flash->open)
    {
        emit info(QString("%1 initialized at %2Hz (SPI mode 0)")
                  .arg(GetDescription(flash))
                  .arg(GetClock(flash)));

        Start(flash);
        FastWrite(flash, RCMD, sizeof(RCMD) - 1);
        FastRead(flash, data, SIZE);
        Stop(flash);

        if(data)
        {
            fp = fopen(FOUT, "wb");
            if(fp)
            {
                fwrite(data, 1, SIZE, fp);
                fclose(fp);

                emit info(QString("Dumped %1 bytes to %2")
                          .arg(SIZE)
                          .arg(FOUT));
                retval = EXIT_SUCCESS;
            }

        }
    }
    else
    {
        emit error(QString("Failed to initialize MPSSE: %1")
                   .arg(ErrorString(flash)));
    }

    Close(flash);

    return retval;
}
//--------------------------------------------------------------------------------
