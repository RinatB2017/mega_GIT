#include "cnrom.h"
#include <memory.h>
#include <string.h>

/*

Bank select ($8000-$FFFF)
7  bit  0
---- ----
xxDD xxCC
  ||   ||
  ||   ++- Select 8 KB CHR ROM bank for PPU $0000-$1FFF
  ++------ Security diodes config

  */


CNROM::CNROM()
{



}


void CNROM::WriteMemory(unsigned short offset, unsigned char val)
{


       //CHRPtr = this->bCHRBanks[(val & 3)];
        this->SwapCHR8((val & 3));

}

byte CNROM::ReadMemory(unsigned short offset)
{
    offset -= (MEM_16k<<1);
    if(offset>=MEM_16k)
    return bPRGBanks[1][offset-MEM_16k];
   // else if(offset<MEM_16k)
    return bPRGBanks[0][offset];

}

void CNROM::Init(unsigned char numPRG, unsigned char numCHR)
{

    bNumPRGBanks = numPRG;
    bNumCHRBanks = numCHR;
    //no bankswitching of PRG on CNROM, so just set up 2x16
    this->bPRGBanks[0] = new byte [MEM_16k];
    memcpy(this->bPRGBanks[0], this->PRG, MEM_16k);
    if(numPRG==1)
    {
        bPRGBanks[1] = bPRGBanks[0];
    }
    else
    {

        this->bPRGBanks[1] = new byte [MEM_16k];
        memcpy(bPRGBanks[1], PRG+MEM_16k, MEM_16k);
    }

    if(numCHR)
    {
        this->SetupCHR(bNumCHRBanks);
    }
    else
    {
        this->SetupVRAM8k();


    }



    //this->CHRPtr = bCHRBanks[0]; //point CHR to first switchable block
    this->SwapCHR8(0);

}
