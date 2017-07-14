#include "uxrom.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

UxROM::UxROM()
{
}

void UxROM::Init(unsigned char numprgbanks, unsigned char numchrbanks)
{
    int i;
    for(i=0;i<numprgbanks;i++)
    {
        //allocate 16kb / bank
        this->bPRGBanks[i] = new byte[MEM_16k];
        memcpy(bPRGBanks[i], PRG+((i)*MEM_16k), MEM_16k);

    }

    this->bNumPRGBanks = numprgbanks;
    this->bNumCHRBanks = numchrbanks;

    if(numchrbanks ==0)
    {
        this->SetupVRAM8k();
    }
    else
        this->SetupCHR(bNumCHRBanks);

    //this->CHRPtr = bCHRBanks[0];
    this->SwapCHR8(0);
    bPRGBanks[bNumPRGBanks+2] = bPRGBanks[numprgbanks-1];//point bank #1 to the last 16kb chunk, this bank is FIXED.
    bPRGBanks[bNumPRGBanks+1] = bPRGBanks[numprgbanks-1]; // this should point to no bank in particular, AFAIK

}

void UxROM::WriteMemory(unsigned short offset, unsigned char val)
{
    //UXROM has a 3 bit bank reg, SxROM (?) is four bits. same mapper number?

    bPRGBanks[bNumPRGBanks+1] = bPRGBanks[val];

}

byte UxROM::ReadMemory(unsigned short offset)
{
    offset -= (MEM_16k<<1);
    if(offset >= 0x4000)
       return bPRGBanks[bNumPRGBanks+2][offset-MEM_16k];
    else
      return  bPRGBanks[bNumPRGBanks+1][offset];


}
