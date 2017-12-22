#include "mapper.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>

Mapper::Mapper()
{
    this->bFourScreen = this->bHorizontal = this->bSingle = this->bVertical = this->bHasSRAM = 0;
    SRAM = 0;
    bNumPRGBanks = bNumCHRBanks = 0;
    wSingleMirroringBase = 0;
    this->SRAM_filename = 0;
    CHRPtr_high = 0;
    CHRPtr_low = 0;

}

Mapper::~Mapper()
{
    //HEAP errors afloat here, can't use delete [], debug later...
int x = bNumPRGBanks -1 ;
while(x>-1)
{
    delete [] this->bPRGBanks[x];
    x--;
}
x = (this->bNumCHRBanks * 2)-1;
while(x>-1)
{
    delete [] this->bCHRBanks[x];
    x--;
}

if(SRAM != 0)
{
    this->SaveSRAMFile();
    delete [] SRAM;
}

if(SRAM_filename)
    delete [] SRAM_filename;

CHRPtr_low = CHRPtr_high = 0;
}

byte *Mapper::GetCPUMemMap()
{
    return (byte *)0xF00CBEEF; //should be able to alert me if I do call it by mistake ^_^
}

byte *Mapper::GetPPUMemMap()
{
   // return (byte *)this->CHRPtr;

}


void Mapper::InitFreshSRAM()
{
    this->SRAM = new byte[0x2000];
    memset(SRAM, 0, 0x2000);
}

void Mapper::WriteSRAM(unsigned short offset, unsigned char val)
{
    if(SRAM == 0)
        this->InitFreshSRAM();


    offset -= 0x6000;
    SRAM[offset] = val;
}

byte Mapper::ReadSRAM(unsigned short offset)
{
    if(SRAM==0)
        this->InitFreshSRAM();

    offset-= 0x6000;
    return SRAM[offset];
}

int Mapper::SaveSRAMFile()
{
    //saves the SRAM to a file (ROM file name + .fsr extension)

    if(!SRAM_filename)
        return 2;
    FILE *fp = fopen(SRAM_filename, "wb");
    if(!fp)
        return FILE_OPEN_ERROR;

    if(fwrite(this->SRAM, 0x2000, 0x1, fp) != 1)
        return FILE_READ_ERROR;
    fclose(fp);
    return 0;

}

int Mapper::LoadSRAMFile(char *file)
{
    FILE *fp = fopen(file, "rb+");

    if(!fp)
    {
                //file didn't exist...
                //save the filename for later
        SRAM_filename = new char [2048];
        memset(SRAM_filename, 0, 2048);
        memcpy(SRAM_filename, file, strlen(file));


        return 0;

    }
    if(!SRAM)
        this->InitFreshSRAM();
    size_t rd = fread(this->SRAM, 0x2000, 1, fp);
    if(rd != 1)
        return FILE_READ_ERROR;

    fclose(fp);
    return 0;

}

void Mapper::SwapPRG16(int _16kbank, int banknumber)
{
    if(banknumber > this->bNumPRGBanks)
    {
        printf("[mapper.cpp]:: Illegal PRG bankswitch");
        return;
    }
    if(_16kbank)
    this->bPRGBanks[bNumPRGBanks + 2] = bPRGBanks[banknumber];
    else
    this->bPRGBanks[bNumPRGBanks + 1] = bPRGBanks[banknumber];
}

void Mapper::SwapCHR8(int banknumber)
{
    //copy 2x0x1000 chunks
  //  memcpy(CHRPtr, bCHRBanks[banknumber], 0x1000);
  //  memcpy(CHRPtr+0x1000, bCHRBanks[banknumber+1], 0x1000);
 //   CHRPtr = bCHRBanks[banknumber];
      CHRPtr_low = bCHRBanks[banknumber];
      CHRPtr_high = bCHRBanks[banknumber + 1];
}

void Mapper::SwapCHR4(int _4kbank, int banknumber)
{

    if(!_4kbank)
        CHRPtr_low = bCHRBanks[banknumber];
    else
        CHRPtr_high = bCHRBanks[banknumber];
   /* if(_4kbank)
        memcpy(CHRPtr+0x1000, bCHRBanks[banknumber], 0x1000);
    else
        memcpy(CHRPtr, bCHRBanks[banknumber], 0x1000);
*/
  /*  if(_4kbank)
        memcpy((CHRPtr + 0x1000), bCHRBanks[banknumber], 0x1000);
    else
        CHRPtr = bCHRBanks[banknumber];
*/
}

byte Mapper::ReadCHR(word offset)
{
    if(offset>=MEM_4k)
        return this->CHRPtr_high[(offset-MEM_4k)];
    else if(offset<MEM_4k)return this->CHRPtr_low[offset];

    else
        printf("CHR read out of reach");
}
void Mapper::WriteCHR(unsigned short offset, unsigned char val)
{
    if(offset>=MEM_4k)
        this->CHRPtr_high[(offset-MEM_4k)] = val;
    else if(offset < MEM_4k)
        this->CHRPtr_low[offset] = val;
    else
        printf("CHR out of bounds");
}

void Mapper::SetupCHR(int num_chrbanks)
{

        int num = num_chrbanks * 2;

    for(int i=0;i<num;i+=2)
    {
        this->bCHRBanks[i] = (byte *)malloc((MEM_4k));
        this->bCHRBanks[i+1] = (byte *)malloc((MEM_4k));

        memcpy(bCHRBanks[i],   this->CHR + (i *     MEM_4k), MEM_4k);
        memcpy(bCHRBanks[i+1], this->CHR + ((i+1) * MEM_4k), MEM_4k);

    }

}

void Mapper::SetupVRAM8k()
{
    bCHRBanks[0] = (byte *)malloc(MEM_4k);
    bCHRBanks[1] = (byte *)malloc(MEM_4k);
}
