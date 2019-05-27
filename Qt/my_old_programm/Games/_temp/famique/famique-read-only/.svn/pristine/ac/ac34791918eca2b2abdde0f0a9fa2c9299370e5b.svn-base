#include "nrom.h"
#include <memory.h>
#include <stdio.h>
#include <string.h>


NROM::NROM()
{
    this->bPRGBanks[0] = new byte[MEM_16k];
    this->bPRGBanks[1] = new byte[MEM_16k];

}


NROM::~NROM()
{


    delete [] bPRGBanks[0];
    delete [] bPRGBanks[1];


}


byte NROM::ReadMemory(word offset)
{

    //calculate relative offset from CPU memory map to the internal cart ROM

    offset -= (MEM_16k<<1);

    if(offset>=0x4000)
        return this->bPRGBanks[1][offset-MEM_16k];
    else
        return this->bPRGBanks[0][offset];

}

void NROM::WriteMemory(unsigned short offset, unsigned char val)
{


 //if nrom reaches here, we've got a bug somewhere
    /* crash the program as a notification ;) */
    printf("NROM recieved a write. highly suspicious. CPU addr:$%04X=$%02X", offset, val);

}


void NROM::Init(unsigned char numPRG, unsigned char numCHR)
{

    //set up PRG and CHR tables for NROM
    memcpy(bPRGBanks[0], PRG, MEM_16k);

    bNumPRGBanks = numPRG;
    bNumCHRBanks = numCHR;
    if(numPRG == 1) // 1x16kb PRG
    {
        bPRGBanks[1] = bPRGBanks[0]; //point the second bank to the first, since they're the same :O
    }
    else
    memcpy(bPRGBanks[1], PRG+MEM_16k, MEM_16k);

    if(bNumCHRBanks)
        this->SetupCHR(bNumCHRBanks);
    else
        this->SetupVRAM8k();

    this->SwapCHR8(0);




}


