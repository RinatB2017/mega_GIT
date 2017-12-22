#include "mmc1.h"
#include <memory.h>
#include <string.h>


mmc1::mmc1()
{

    for(int i=0;i<32;i++)
    {
        this->bCHRBanks[i] = 0;
        this->bPRGBanks[i] = 0;
    }


    this->InitFreshSRAM();
    rd0=rd1=rd2=rd3 = 0;
    this->hidden_register = this->hidden_register_count = 0;

}

mmc1::~mmc1()
{
   // if(this->bNumCHRBanks != 0)
 //   delete [] CHRPtr_low;
  //  delete [] CHRPtr_high;

}

#ifdef MMC_NONWORKING
void mmc1::WriteMemory(unsigned short offset, unsigned char val)
{


    if((val & 0x80)==0x80)
    {
        //if the seventh bit is set means we're dealing with a
        hidden_register = 0;
        hidden_register_count = 0;

     //   memset(&this->mmc1_control, 0, sizeof(struct control_reg));


        mmc1_control.prgsize = 1;
        mmc1_control.banksel = 1;
        //this also locks the last PRG bank to $c000
      //  this->bPRGBanks[this->bNumPRGBanks+2] = this->bPRGBanks[bNumPRGBanks-1];
    }

   else if(!this->hidden_register_count)
    {
        //this is the first (out of five) writes to set the bankswitch
        this->hidden_register=0;

        this->hidden_register = (val & 0x1);
        hidden_register_count++;
    }
   else if(this->hidden_register_count  < 4)
   {
       hidden_register <<=1;
       hidden_register |= (val & 0x1);
       hidden_register_count++;
   }
   /*   $8000-9FFF:  [...C PSMM]
    C = CHR Mode (0=8k mode, 1=4k mode)
    P = PRG Size (0=32k mode, 1=16k mode)
    S = Slot select:
        0 = $C000 swappable, $8000 fixed to page $00 (mode A)
        1 = $8000 swappable, $C000 fixed to page $0F (mode B)
        This bit is ignored when 'P' is clear (32k mode)
    M = Mirroring control:
        %00 = 1ScA
        %01 = 1ScB
        %10 = Vert
        %11 = Horz


  $A000-BFFF:  [...C CCCC]
    CHR Reg 0

  $C000-DFFF:  [...C CCCC]
    CHR Reg 1

  $E000-FFFF:  [...W PPPP]
    W = WRAM Disable (0=enabled, 1=disabled)
    P = PRG Reg
    */

   else if(this->hidden_register_count == 4) // the fifth write sets the destinatoin adress, and thus determines the action of the mapper
   {

       hidden_register_count = 0;

       if(offset>=0x8000 && offset <=0x9fff) //this address range indicates that we're dealing with the control register of the MMC1
       {

           mmc1_control.chrsize = (this->hidden_register & 0x10) ? true:false;
           mmc1_control.prgsize = (this->hidden_register & 0x8) ? true:false;
           mmc1_control.banksel = (this->hidden_register & 0x4) ? true:false;
            mmc1_control.ppu_mirroring = (this->hidden_register & 0x3); //test the two lower bits
            //mmc1_control.modified = true;


       }

       if(offset >= 0xA000 && offset <= 0xBFFF)
       { //set chr bank

               if(mmc1_control.chrsize == 1)//4kb

               {


               }

               else //8kb

               {


               }



       }
       else if(offset>=0xc00 && offset <= 0xdfff)
       {


       }
       else if(offset >=0xe000 && offset <= 0xFFFF) //PRG SELECT
       {

           byte bank = 0;

                if(mmc1_control.prgsize) //16kb
                {
                    bank = hidden_register & 0xf;
                    if(mmc1_control.banksel == 1)
                    {
                        //swap at $8000
                        bPRGBanks[this->bNumPRGBanks+1] = bPRGBanks[bank];
                        bPRGBanks[this->bNumPRGBanks+2] = bPRGBanks[0xf];
                    }
                    else
                    {
                        bPRGBanks[this->bNumPRGBanks+2] = bPRGBanks[bank];
                        bPRGBanks[bNumPRGBanks+1] = bPRGBanks[0];

                        //swap at $c000
                    }

                }
                else
                {
                    bank = (hidden_register <<4)>>5; //ignore lower bit
                    //32 kb
                    this->bPRGBanks[this->bNumPRGBanks+1] = bPRGBanks[bank];
                    this->bPRGBanks[this->bNumPRGBanks+2] = bPRGBanks[bank+1];

                }




       }



   }


}


void mmc1::WriteMemory(unsigned short offset, unsigned char val)
{

    if( (val & 0x80) == 0x80)
    {
        //reset MMC registers
        this->hidden_register = this->hidden_register_count = 0;


        //this->rd0 = this->rd1 = this->rd2 = this->rd3 = 0;
        rd0 |= 0x0C;//= ((1<<3) | (1<<2));

        //upon reset the $c000 <---> last bank
        Synchronize();

//        this->bPRGBanks[this->bNumPRGBanks+2] = this->bPRGBanks[bNumPRGBanks-1];

    }

    else
    {
        hidden_register |= (val&0x1) << hidden_register_count;
        hidden_register_count++;

        if(hidden_register_count == 5)
        {



        //see if mapper changed PPU mirroring




            if(offset >= 0x8000 && offset <= 0x9FFF)

            {

                rd0 = hidden_register;

                this->bSingle = this->bHorizontal = this->bVertical = 0;
                switch(rd0 & 0x3) //update PPU mirror
                {
                case 0:
                case 1:

                    //onescreen!

                    this->bSingle = true;
                    this->wSingleMirroringBase = ((rd0 & 0x1) ? 0x400 : 0);

                    break;
                case 2:
                    this->bVertical = true;
                    break;
                case 3:
                    this->bHorizontal = true;
                    break;

                }


            }
            else if(offset >= 0xA000 && offset <= 0xBFFF)
            {
                rd1 = hidden_register;

                //do voodoo 4kb bankcopying
              /*  if((rd0 & 0x10) == 0x10)
                {
                    memcpy(CHRPtr, this->bCHRBanks[rd1], MEM_8k);

                }
                else
                {
                    if(this->bNumCHRBanks == 0)
                        memcpy(CHRPtr,SRAM, MEM_8k);
                    else
                        memcpy(CHRPtr, bCHRBanks[rd1], MEM_8k);
                    // CHRPtr = SRAM;
                }
*/
                if(this->bNumCHRBanks)

                    this->SwitchCHR();
                else
                    this->SwitchSRAM();

                // CHRPtr = SRAM;//normal 8kb bankswitch
            }
            else if(offset >= 0xC000 && offset <= 0xDFFF)
            {
                rd2 = hidden_register;

                 this->SwitchCHR();
            }
            else if(offset >= 0xE000 && offset <= 0xFFFF)
            {
                this->rd3 = (hidden_register & 0xF);


                /*  if( (rd0&0x8)) //controls PRG bank size
            {
                 //16kb
                if(! (rd0&0x4) )
                {
                    //switch out $8000
                    this->bPRGBanks[this->bNumPRGBanks+1] = this->bPRGBanks[rd3];
                    this->bPRGBanks[this->bNumPRGBanks+2] = this->bPRGBanks[this->bNumPRGBanks-1];

                }
                else
                {
                    this->bPRGBanks[this->bNumPRGBanks+2] = this->bPRGBanks[rd3];
                    this->bPRGBanks[this->bNumPRGBanks+1] = this->bPRGBanks[0];
                    //switch out $c000
                }

            }
            else
            {
                byte bank_offset = (rd3 >> 1) * 2;
                this->bPRGBanks[this->bNumPRGBanks+1] = this->bPRGBanks[bank_offset];
                this->bPRGBanks[this->bNumPRGBanks+2] = this->bPRGBanks[bank_offset+1];


                //32 kb



            }*/

                if(!(rd0 & 0x8)) //swaps entire 32kb PRG space
                {
                    this->bPRGBanks[this->bNumPRGBanks+1] = this->bPRGBanks[(rd3 & 0xE)]; //skip first (low) bit when setting 32kb ?
                    this->bPRGBanks[this->bNumPRGBanks+2] = this->bPRGBanks[(rd3 & 0xE) | 1] ;
                }




                else

                {
                    if((rd0&0x4) == 0x4)
                    {
                        //switch out $8 000
                        this->bPRGBanks[this->bNumPRGBanks+1] = this->bPRGBanks[rd3];
                        this->bPRGBanks[this->bNumPRGBanks+2] = this->bPRGBanks[this->bNumPRGBanks-1];


                    }
                    else
                    {

                        this->bPRGBanks[this->bNumPRGBanks+1] = this->bPRGBanks[0];
                        this->bPRGBanks[this->bNumPRGBanks+2] = this->bPRGBanks[rd3];
                        //switch out $c000


                    }


                }



            }

        hidden_register_count = 0;
        this->hidden_register = 0;


    }

    }



}
#endif


void mmc1::WriteMemory(unsigned short offset, unsigned char val)
{

    if( (val & 0x80) == 0x80)
    {
        //reset MMC registers
        this->hidden_register = this->hidden_register_count = 0;

        rd0 |= 0x0C;//= ((1<<3) | (1<<2));
       // rd1 = rd2 = rd3 = 0;
        this->SwitchPRG();
        SwitchCHR();
        Mirroring();


    }
    else
    {
        hidden_register |= (val&0x1) << hidden_register_count;
        hidden_register_count++;

        if(hidden_register_count == 5)
        {

            if(offset >= 0x8000 && offset <= 0x9FFF)
            {
                rd0 = hidden_register;

                this->Mirroring();


                this->SwitchPRG();
            }
            else if(offset>=0xA000 && offset <= 0xBFFF)
            {
                rd1 = hidden_register & 0x1f;


                this->SwitchCHR();
                this->SwitchPRG();

            }
            else if(offset >=0xC000 && offset <= 0xDFFF)
            {
                rd2 = hidden_register & 0x1F;
                this->SwitchCHR();
            }
            else if(offset >=0xE000 && offset <= 0xFFFF)
            {
                rd3 = hidden_register;
                this->SwitchPRG();
            }
            hidden_register = 0;
            hidden_register_count = 0;


        }
    }
}


byte mmc1::ReadMemory(unsigned short offset)
{
    //first read will be to determine the execution vector, in the bank at $e000-$ffff


    offset -= (MEM_16k<<1);
    if(offset>=0x4000)
        return bPRGBanks[this->bNumPRGBanks+2][offset-MEM_16k]; //HIGH PRG
    else
        return bPRGBanks[this->bNumPRGBanks+1][offset]; //LOW PRG

}

void mmc1::Init(unsigned char num_16kb, unsigned char num_8kb)
{


    //for this mapper we'll use a banksize of 8kb

bNumPRGBanks = num_16kb;
bNumCHRBanks = num_8kb;

    for(int i=0;i<(bNumPRGBanks);i++)
    {
        this->bPRGBanks[i] = new byte[MEM_16k];
        memcpy(bPRGBanks[i], PRG+((MEM_16k) * i), MEM_16k);
    }
/*
    for(int j=0;j<(num_8kb*2);j++)
    {//we're reading in 4kb chunks of CHR, this is the minimum swappable CHR memory area
        this->bCHRBanks[j] = new byte[MEM_8k>>1];
        memcpy(bCHRBanks[j], CHR+((MEM_8k>>1)*j), MEM_8k>>1);
    }
*/
    if(bNumCHRBanks)
        this->SetupCHR(bNumCHRBanks);
    else
        this->SetupVRAM8k();


/* IF MMC1 GAMES REFUSE TO BOOT, MAKE SURE THE INITIAL BANKS ARE CORRECTLY SET UP

   here I switch in the last 16kb bank @ 0xC000, and (I believe this is completely irrelevant) the first bank at 0x8000


*/


 //   this->bPRGBanks[bNumPRGBanks+2] = bPRGBanks[bNumPRGBanks-1];
   // this->bPRGBanks[bNumPRGBanks+1] = bPRGBanks[0];


    rd0 |= 0x0C;
    this->SwapPRG16(1, bNumPRGBanks-1);
    this->SwapCHR8(0);
   // this->SwitchCHR();

}

void mmc1::Synchronize()
{
    SwitchPRG();
/*

    if(this->bNumCHRBanks == 0)
        this->SwitchSRAM();
    else
        this->SwitchCHR();


    this->Mirroring();
*/}

#define CALC_HIGH_CHR \
if(rd0&0x10) high_bank = (rd2 & 0x1f);else high_bank = (rd1&0x1e)|1
#define CALC_LOW_CHR \
if(rd0&0x10) low_bank = (rd1 & 0x1f);else low_bank = rd1 & 0x1e


void mmc1::SwitchCHR()
{

    if(this->bNumCHRBanks)
    {

        if((rd0&0x10) == 0x10)
        {
            this->SwapCHR4(0, (rd1&0x1f));
            this->SwapCHR4(1, (rd2&0x1f));
        }
        else
        {
            this->SwapCHR4(0, (rd1 & 0x1e)|0);
            this->SwapCHR4(1, (rd1 & 0x1e)|1);
        }

    }
    else
    {
        if((rd0&0x10) == 0x10)
            printf("mmc1: chr-ram swap 4k");
        else
            printf("mmc1: chr-ram swap 8k");

        printf("[MMC1.cpp]:CHR RAM swap?");

        //CHR RAM
    }
}

void mmc1::SwitchPRG()
{
    byte high_bank = 0;
    byte low_bank = 0;

    byte offset = rd1 & 0x10;
    if(rd3 & 0x10)
        printf("MMC1: WRAM swapping expected...");

    int prgbanksoffs = this->bNumPRGBanks -1;

    if(rd0 & 0x08)
    {
        if(rd0&0x04)
        {
            this->SwapPRG16(0, (rd3&0xF));
            this->SwapPRG16(1, prgbanksoffs);
        }

        else
        {

            this->SwapPRG16(0, 0);
            this->SwapPRG16(1, (rd3&0xF));
        }

    }
    else
        {

            this->SwapPRG16(0, (rd3&0xE)|0);
            this->SwapPRG16(1, (rd3&0xE)|1);
        }



  /*

    if( (rd0 & 8) == 8)
    {
        if( (rd0 & 4) == 4)
        {
            low_bank = rd3 & 0xF;
            high_bank = this->bNumPRGBanks-1;
        }
        else
        {
            low_bank = 0;
            high_bank = rd3 & 0xF;
        }
    }
    else
    {
        low_bank = (rd3 & 0xE);
        high_bank = (rd3 & 0xE)|1;
    }


    this->bPRGBanks[this->bNumPRGBanks+1] = bPRGBanks[low_bank];
    this->bPRGBanks[this->bNumPRGBanks+2] = bPRGBanks[high_bank];


*/

}

void mmc1::Mirroring()
{
    byte mirroring = rd0 & 0x3;
    bSingle = bVertical = bHorizontal = 0;

    switch(mirroring)
    {
    case 0:
        this->bSingle = true;
        this->wSingleMirroringBase = 0x0;
        break;
    case 1:
        this->bSingle = true;
        this->wSingleMirroringBase = 0x400;
        break;
    case 2:
        this->bVertical = true;
        printf("vertical    ");
        break;
    case 3:
        this->bHorizontal = true;
        printf("horizontal ...");
        break;

    }

}

void mmc1::SwitchSRAM()
{

 /*   byte *tmp_bfr1 = new byte[0x1000];
    byte *tmp_bfr2 = new byte[0x1000];
    byte high_bank, low_bank;
    CALC_HIGH_CHR;
    CALC_LOW_CHR;
    if(low_bank==high_bank)
    {
        memcpy(CHRPtr+0x1000, CHRPtr, 0x1000);


    }


    delete [] tmp_bfr1;
    delete [] tmp_bfr2;
    */
    /*byte high_bank, low_bank;
    CALC_HIGH_CHR;
    CALC_LOW_CHR;

    memcpy(CHRPtr, SRAM+(low_bank * 0x1000), 0x1000);

    memcpy(CHRPtr+0x1000, SRAM+ 0x1000, 0x1000);

    memcpy(CHRPtr, SRAM, MEM_8k);
*/

}
