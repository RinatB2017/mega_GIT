#include "memory.h"
#include "cpu/cpu2.h"
#include <string.h>
//#include "SDL/SDL.H"
//#include "apu.h"

#define _OLD_MEMORY_MANAGER


MemoryManager::MemoryManager(NESCart *cart)
{
        RAM6502 = new byte [0x800]; //allocate the onboard RAM
        memset(RAM6502, 0xFF, 0x800);


      //  memset(RAM6502, 0, 0x800);

        memset(&control1, 0, sizeof(controller));
        memset(&control2, 0, sizeof(controller));




        //uAPU = new APU();

        //uAPU->SetMemManPtr(this); //set the pointer to this instance of MM, needed for DMC playback.
        this->cartptr = cart;








}



MemoryManager::~MemoryManager()
{
        delete []RAM6502;

        //delete uAPU;




}
#ifdef _NEW_MEMORY_MANAGER_CART

byte MemoryManager::ReadMemory(unsigned short offset)
{


}

#endif
#ifdef _OLD_MEMORY_MANAGER
byte MemoryManager::ReadMemory(word offset)
{


    if(offset < 0x2000)
    {

            return this->RAM6502[offset &0x7ff];
    }

        if(offset >= 0x2000 && offset < 0x3FFF) //PPU registers + mirrors
        {
            if(offset == 0x3ffa)
                printf("över 2007");
            if(offset == 0x2022)
                printf("blah");

            return uCPU->ppu_2->ReadMemory(0x2000 + (offset&0x7));
               // return uCPU->ppu.read_ppu( 0x2000+ (offset&0x7));

        }


        if(offset == 0x4016)
        {
            byte ret;
            ret = ((control1.controller >> control1.control_read_bit) & 1) | 0x40;
            control1.control_read_bit++;
            return ret;

        }
        if(offset == 0x4017)
        {
            byte ret;
            ret = ((control2.controller >> control2.control_read_bit)&1)|0x40;
            control2.control_read_bit++;
            return ret;
                //controller sheit
        //	return 1;
        }

        if(offset == 0x4015)
        {
            //return uAPU->blarggAPU->read_status(uAPU->elapsed());

        }
        if(offset >=0x4018 && offset <= 0x5fff)
        {
            return 0x00; //ugh. no open bus yet.
        }

        if(offset >=0x6000 && offset <=0x7fff)
        {
          return cartptr->mROMMapper->ReadSRAM(offset);
        }



        else if(offset>=0x8000 && offset <=0xFFFF) //PRG reads through the mapper
            return cartptr->mROMMapper->ReadMemory(offset);


        else
            printf("unauthourized memory access [PC:$%04X], offset $%04X", uCPU->GetCPUState().pc, offset);


}

/*signed char MemoryManager::ReadMemorySigned(word offset)
{

        return (signed char)memory[offset];

}*/

void MemoryManager::DMA_transfer(unsigned short offset, unsigned char data)
{

    word oam_offset = 0;
    //printf("performing DMA copying to PPU memory [OAM]\n");
    //memcpy((byte *)&ppu.OAM,(byte *) &memory[value * 0x100], 0xFF);



//	uCPU->ppu.reg_2003 = 0;
    /*

    for(int i=oam_offset;i<256;i++)
    {
//        uCPU->ppu.write_ppu(0x2004, ReadMemory((data*0x100)+i));

__ugly_timing_hack:
            if(uCPU->cycles<NTSC_CYCLES_PER_HBLANK)
            {


                uCPU->ppu_2.WriteMemory(0x2004, ReadMemory((data*0x100)+i));
                   // uCPU->ppu.write_ppu(0x2004, ReadMemory( (data * 0x100) + i));


            //uCPU->CallOpCode(0x8D); //STA absolute
                    uCPU->cycles += 2; //2004 is always accessed through absolute addressing mode?
                                       //perhaps it can be used with ABS,X? need to investigate...
            }
            else
            {
                    uCPU->cycles = 0;
                    uCPU->ppu_2.do_scanline();
                    goto __ugly_timing_hack;
            }


    }

from emulator.cpp:
cpu->memory->uAPU->cpu_cycles_remain > 0)
        {

            if(CheckBP())
                break;
            //In HSYNC - run CPU for the remainder of the HSYNC
            if(cpu->CPU_crash)
            {
                if(QMessageBox::warning(NULL, "FATAL", "The CPU has reached an unrecoverable state and will now die\nFatal PC:$"+
                                     QString::number(cpu->previous_state.pc, 16)+"\nCurrent PC:$"+
                                     QString::number(cpu->GetCPUState().pc, 16)+QString("\nWould you like to run the CPU for another cycle to see if the problem resolves itself? (emulator might crash)"),
                                     QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                    {
                    this->GetCPUHandle()->CPU_crash = false;
                }
                else
                {
                Stop();
                PrintDebugString(QString("CPU crashed at PC=$")+QString::number(this->GetCPUHandle()->previous_state.pc, 16) + ". This might be due to a bad ROM dump or due to an emulator bug");
                break;}

            }
            cpu->NextOpCode();

            cpu->memory->uAPU->cpu_cycles_remain -= cpu->GetCycleCount();





*/

    word mem_offset = (data *0x100);
// uCPU->ppu_2.WriteMemory(0x2003, 0x00); //reset OAM counter
   // uCPU->ppu_2.Reset2003();


   uCPU->ppu_2->Reset2003(); //THIS BREAKS NEStress
/*
   for(int i=0;i<0x100;i++)
   {

       _apu_timing_hack:
       if(uAPU->cpu_cycles_remain > 0)
       {
       uCPU->ppu_2->WriteMemory(0x2004, ReadMemory(mem_offset+i));
       uAPU->cpu_cycles_remain -= 2; //two CPU cycles per copy.

        }
       else
       {
           uCPU->ppu_2->do_scanline();
           if(uCPU->ppu_2->draw)
           {

               uAPU->RenderAudio(uAPU->elapsed());

               uCPU->ppu_2->draw = false;




           }
           uAPU->cpu_cycles_total += NTSC_CYCLES_PER_HBLANK;//cpu->NumCycles();
           uAPU->cpu_cycles_remain += NTSC_CYCLES_PER_HBLANK; //cpu->NumCycles();

           goto _apu_timing_hack;
       }
    }
    */
    for(int i=0; i<0x100;i++)
        uCPU->ppu_2->WriteMemory(0x2004, ReadMemory(mem_offset+i));

 //   uAPU->cpu_cycles_remain -= 512;
    uCPU->cycles += 512;




}

void MemoryManager::WriteMemory(word offset, byte data)
{
    if(offset < 0x2000) //memory mirroring every 0x800 bytes
    {
            this->RAM6502[offset & 0x7FF] = data;

    }
      else  if(offset >= 0x2000 && offset <= 0x2007) //PPU
        {
                //printf("PC:$%04X:: PPU Write @ $%04X \n",PC, adress);
             //   uCPU->ppu.write_ppu(offset, data);

            uCPU->ppu_2->WriteMemory(offset, data);
        //	memory[adress] = value;
        }
        else if(offset>=0x2008 && offset < 0x3FFF)
        {
            uCPU->ppu_2->WriteMemory( 0x2000 + (offset & 0x7), data);
                printf("[cpu2.cpp]:attempting to write to one of the unimplemented PPU mirrors...\n");


        }

        else if(offset == 0x4014)
        {
            this->DMA_transfer(offset, data);
        }

        else if (offset == 0x4016)
        {

                if(control1.latch && ((data & 1) ==0))
                {
                        control1.control_read_bit = 0;
                        control2.control_read_bit = 0;
                }
                control1.latch = (data & 1);

        }

        /*if(offset >= uAPU->blarggAPU->start_addr && offset <= uAPU->blarggAPU->end_addr)
        {
            uAPU->blarggAPU->write_register(uAPU->elapsed(), offset, data);
            //sound shite

        }*/




        else if (offset >=0x6000 && offset <= 0x7FFF)
        {
            cartptr->mROMMapper->WriteSRAM(offset, data);

        }
        else if (offset >=0x8000 && offset <= 0xFFFF)
        {

            cartptr->mROMMapper->WriteMemory(offset, data);

            //mapper shit goes here
        //        uCPU->PrintDebug("don't write to PRG, silly motherfucker!\n");

                //merely for zeee testing

        }
/*	else
                memory[offset] = data;
*/
}
#endif
