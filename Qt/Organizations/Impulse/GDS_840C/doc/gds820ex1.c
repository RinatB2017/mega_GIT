/* Filename
- gds820ex1.c
*
* This is an example program written in C. We use a NI's GPIB interface
* card and one X86 PC to control GDS-820/GDS-840. This program could
* get the waveform data from GDS-820/GDS-840, andsave them to a file.
* You can use Microsoft Visual C++ or Borland C++ Builder to compile this 
* file. And you must link this file with an object file
* (BORLANDC_GPIB-32.OBJ) for Borland C++ or GPIB-32.OBJ for Visual C++ )
* that provided by National Instruments Corporation. DECL-32.H is a
* Win32 C/C++ include file, that contains NI-488.2 function prototypes
* and various pre-defined constants. It's also provided by NI.
*
* Copyright GOOD WILL INSTRUMENT
* All Rights Reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
* Include the WINDOWS.H and DECL-32.H files. The standard Windows
* header file, WINDOWS.H, contains definitions used by DECL-32.H and
* DECL-32.H contains prototypes for the GPIB routines and constants.
*/
#include "decl-32.h"
#define ARRAYSIZE 1100
/* Size of read buffer */
#define BDINDEX 0 /* Board Index */
#define PRIMARY_ADDR_OF_DMM 7
/* Default primary address of GDS820/840 */
#define NO_SECONDARY_ADDR 0
/* Secondary address of device */
#define TIMEOUT T3s
/* Timeout value = 5 seconds */
#define EOTMODE 1 /* Enable the END message */
#define EOSMODE 0 /* Disable the EOS mode */
void Acquire(void);
void Delay(int);
void ShowIbsta(char);
int Dev;
/* Device handle */
unsigned char ReadBuffer[201];
/*
Read data buffer
*/
char ErrorMnemonic[21][5] = {"EDVR", "ECIC", "ENOL", "EADR", "EARG",
                             "ESAC", "EABO", "ENEB", "EDMA", "",
                             "EOIP", "ECAP", "EFSO", "", "EBUS",
                             "ESTB", "ESRQ", "", "", "", "ETAB"};
unsigned char WaveBuf[ARRAYSIZE];
int main(void)
{
    int i, count=0;
    unsigned char ch;
    Dev
            = ibdev (BDINDEX, PRIMARY_ADDR_OF_DMM, NO_SECONDARY_ADDR,
                     TIMEOUT, EOTMODE, EOSMODE);
    if(ibsta & ERR)
        printf("\n\rUnable to open device");
    ibclr(Dev);
    if(ibsta & ERR)
        printf("\n\rUnable to clear device(ibsta= %x), (iberr=%x)",ibsta,iberr);
    ibwrt(Dev, "*CLS\n", 5L);
    /*Clear ths status registers and*/
    printf("\n\r*CLS\n\r");
    /*Output buffer of GDS-820/GDS-840. */
    if((ibsta&ERR)||(ibsta&TIMO)){
        ShowIbsta(1);
        return 0;
    }
    Delay(1000);
    ibwrt(Dev, "*IDN?\n", 6L);
    /*Get the unique identification */
    printf("*IDN?\n\r");
    /*code of the GDS-820/GDS-840. */
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    while(1)
    {
        ibrd(Dev, ReadBuffer, 100);
        /* Read datas from input bufer. */
        if((ibsta & ERR)||(ibsta & TIMO))
        {
            ShowIbsta(0);
            return 0;
        }
        for(i=0;i<ibcntl;i++)
        {
            ch=ReadBuffer[i];
            WaveBuf[count++]=ch;
        }
        if(ch=='\n')
        {
            WaveBuf[count]=0x00;
            printf("%s\n\r",WaveBuf);
            break;
        }
    }
    Delay(1000);
    ibwrt(Dev, ":RUN\n", 5L);
    /*Let the GDS-820/GDS-840 run. */
    printf(":RUN\n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }

    Delay(1000);
    ibwrt(Dev,":CHANnel1:DISPlay 1\n", 20L);
    /*Set channel 1 display on */
    printf(":CHANnel1:DISPlay 1\n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    Delay(1000);
    ibwrt(Dev, ":TIMebase:SCALe 2.0e-4\n", 23L); /* Set timebase: 200us/div */
    printf(":TIMebase:SCALe 1.0e-4\n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    Delay(1000);
    ibwrt(Dev, ":CHANnel1:OFFSet 0\n", 19L);
    /* Set offset voltage: 0V */
    printf(":CHANnel1:OFFSet 0\n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    Delay(1000);
    ibwrt(Dev, ":CHANnel1:SCALe 0.5 \n", 20L);
    /* Set vertical scale: 500mV/div */
    printf(":CHANnel1:SCALe 0.5\n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    Delay(1000);
    ibwrt(Dev, ":ACQuire:MODe 0\n", 16L);
    /*Set acquire mode: sample mode */
    printf(":ACQuire:MODe 0\n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    Delay(1000);
    ibwrt(Dev, ":TRIGger:LEVel 0.3\n", 19L);
    /* Set trigger level: 0.3V */
    printf(":TRIGger:LEVel 0.3\n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    Delay(1000);
    ibwrt(Dev, ":TRIGger:MODe 1\n", 16L);
    /* Set trigger mode: AUTO */
    printf(":TRIGger:MODe1 \n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    Delay(10000);
    /* Waiting for acquisition process and GDS-820/GDS-840 internal */
    /* process. The delay time depend on the timebase that */
    /* you selected. Low speed division require much more */
    /* time for acquisition. It's recommended a minimum */
    /* 200ms delay time after the last command, before */
    /* getting the waveform data from GDS-820/GDS-840. */
    Acquire();
    /*Get waveform data. */
    ibwrt(Dev,":SYSTEM:UNLOCK \n",15L);
    /* Unlock GDS-820/GDS-840 from remote control.*/
    printf(":SYSTEM:UNLOCK\n\r");
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ShowIbsta(1);
        return 0;
    }
    ibonl(Dev,0);
    /*Take the device offline. */
    return 1;
}

void Acquire(void)
{
    short i, j;
    FILE *writeP;
    char writeFilename[15] = "wavedata.txt";
    short wave;
    int tmp, count=0;
    ibwrt(Dev, ":ACQUIRE1:POINT \n", 16L);
    printf(":ACQUIRE1:POINT \n\r");
    Delay(1000);
    if((ibsta & ERR)||(ibsta & TIMO))
    {
        ibclr(Dev);
        ibwrt(Dev, "*CLS \n", 5L);
        ShowIbsta(1);
        Delay(100000);
        ibwrt(Dev, ":ACQUIRE1:POINT \n", 16L); /* Try again! */
        printf(":ACQUIRE1:POINT\n\r");
        if((ibsta & ERR)||(ibsta & TIMO))
        {
            ShowIbsta(1);
            Delay(100000);
            exit (1);
        }
    }
    while(1)
    {
        ibrd(Dev, ReadBuffer, 100);
        if((ibsta & ERR)||(ibsta & TIMO))
        {
            ShowIbsta(0);
            Delay(100000);
            exit(1);
        }
        for(i=0;i<ibcntl;i++)
            WaveBuf[count++]=ReadBuffer[i];
        printf("\rReceived: %7d ", count);
        if(count>=1014)
        {
            printf("\rReceived: %7d bytes.\n\r", count);
            break;
        }
    }
    /* Open file "wavedata.txt" and write waveform datas to it. */
    writeP=fopen(writeFilename, "w");
    if(writeP == NULL)
    {
        printf("error: cannot write '%s' \n", writeFilename);
        exit(1);
    }
    j=14;
    for(i=0;i<500;i++)
    {
        wave=WaveBuf[j++]<<8;
        wave+=WaveBuf[j++];
        fprintf(writeP, "%d\n", wave);
    }
    tmp = fclose(writeP);
    if(tmp== EOF)
    {
        printf("error: cannot write '%s'\n\r", writeFilename);
        exit(1);
    }
}

void Delay(int i)
{
    int j,k;
    for(j=0; j<i; j++)
    {
        for(k=0; k<30000; k++);
    }
}

void ShowIbsta(char c)
{
    if(ibsta & ERR)
    {
        if(c) printf("\n\rUnable write to device(ibsta= %x),(iberr=%x)\n\r",ibsta,iberr);
        else
            printf("\n\rUnable read from device(ibsta= %x), (iberr=%x)\n\r",ibsta,iberr);
    }
    else
    {
        if(c)
            printf("\n\rWrite, ibsta= %x", ibsta);
        else
            printf("\n\rRead, ibsta= %x", ibsta);
    }
}

