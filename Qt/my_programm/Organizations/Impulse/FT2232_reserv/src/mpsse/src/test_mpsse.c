#include <stdio.h>
#include <stdlib.h>
#include <mpsse.h>
//--------------------------------------------------------------------------------
#define SIZE	0x8000		// Size of EEPROM chip (32KB)
#define WCMD	"\xA0\x00\x00"	// Write start address command
#define RCMD	"\xA1"		// Read command
#define FOUT	"eeprom.bin"	// Output file
//--------------------------------------------------------------------------------
int test_bitbang(void)
{
    struct mpsse_context *io = NULL;
    int i = 0, retval = EXIT_FAILURE;

    io = MPSSE(BITBANG, 0, 0);

    if(io && io->open)
    {
        for(i=0; i<10; i++)
        {
            PinHigh(io, 0);
            printf("Pin 0 is: %d\n", PinState(io, 0, -1));
            sleep(1);

            PinLow(io, 0);
            printf("Pin 0 is: %d\n", PinState(io, 0, -1));
            sleep(1);
        }

        retval = EXIT_SUCCESS;
    }
    else
    {
        printf("Failed to open MPSSE: %s\n", ErrorString(io));
    }

    Close(io);

    return retval;
}
//--------------------------------------------------------------------------------
int test_ds1305(void)
{
    struct mpsse_context *ds1305 = NULL;
    int sec = 0, min = 0, retval = EXIT_FAILURE;
    char *control = NULL, *seconds = NULL, *minutes = NULL;

    if((ds1305 = MPSSE(SPI3, ONE_HUNDRED_KHZ, MSB)) != NULL && ds1305->open)
    {
        SetCSIdle(ds1305, 0);

        printf("%s initialized at %dHz (SPI mode 3)\n", GetDescription(ds1305), GetClock(ds1305));

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
            sleep(1);

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

            printf("%.2d:%.2d\n", min, sec);

            free(minutes);
            free(seconds);
        }
    }
    else
    {
        printf("Failed to initialize MPSSE: %s\n", ErrorString(ds1305));
    }

    Close(ds1305);

    return retval;
}
//--------------------------------------------------------------------------------
int test_i2ceeprom(void)
{
    FILE *fp = NULL;
    char *data = NULL;
    int retval = EXIT_FAILURE;
    struct mpsse_context *eeprom = NULL;

    if((eeprom = MPSSE(I2C, FOUR_HUNDRED_KHZ, MSB)) != NULL && eeprom->open)
    {
        printf("%s initialized at %dHz (I2C)\n", GetDescription(eeprom), GetClock(eeprom));

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

                        printf("Dumped %d bytes to %s\n", SIZE, FOUT);
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
        printf("Failed to initialize MPSSE: %s\n", ErrorString(eeprom));
    }

    Close(eeprom);

    return retval;
}
//--------------------------------------------------------------------------------
int test_gpio(void)
{
    struct mpsse_context *io = NULL;
    int i = 0, retval = EXIT_FAILURE;

    io = MPSSE(GPIO, 0, 0);

    if(io && io->open)
    {
        for(i=0; i<10; i++)
        {
            PinHigh(io, GPIOL0);
            printf("GPIOL0 State: %d\n", PinState(io, GPIOL0, -1));
            sleep(1);

            PinLow(io, GPIOL0);
            printf("GPIOL0 State: %d\n", PinState(io, GPIOL0, -1));
            sleep(1);
        }

        retval = EXIT_SUCCESS;
    }
    else
    {
        printf("Failed to open MPSSE: %s\n", ErrorString(io));
    }

    Close(io);

    return retval;
}
//--------------------------------------------------------------------------------
int test_spiflash(void)
{
    FILE *fp = NULL;
    char *data = NULL, *data1 = NULL;
    int retval = EXIT_FAILURE;
    struct mpsse_context *flash = NULL;

    if((flash = MPSSE(SPI0, TWELVE_MHZ, MSB)) != NULL && flash->open)
    {
        printf("%s initialized at %dHz (SPI mode 0)\n", GetDescription(flash), GetClock(flash));

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

                printf("Dumped %d bytes to %s\n", SIZE, FOUT);
                retval = EXIT_SUCCESS;
            }

            free(data);
        }
    }
    else
    {
        printf("Failed to initialize MPSSE: %s\n", ErrorString(flash));
    }

    Close(flash);

    return retval;
}
//--------------------------------------------------------------------------------
int test_spiflashfast(void)
{
    FILE *fp = NULL;
    char data[SIZE] = { 0 };
    int retval = EXIT_FAILURE;
    struct mpsse_context *flash = NULL;

    if((flash = MPSSE(SPI0, TWELVE_MHZ, MSB)) != NULL && flash->open)
    {
        printf("%s initialized at %dHz (SPI mode 0)\n", GetDescription(flash), GetClock(flash));

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

                printf("Dumped %d bytes to %s\n", SIZE, FOUT);
                retval = EXIT_SUCCESS;
            }

        }
    }
    else
    {
        printf("Failed to initialize MPSSE: %s\n", ErrorString(flash));
    }

    Close(flash);

    return retval;
}
//--------------------------------------------------------------------------------
