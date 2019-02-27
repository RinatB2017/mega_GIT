#ifndef MEMORY_H
#define MEMORY_H


#include "famicom.h"
//#include "apu/apu.h"

#include "nescart.h"


class M6502; // 6502 class, bitch
class APU; //noise generator

typedef struct input_struct
{

        byte controller;
        byte latch;
        byte control_read_bit;
}controller;


class MemoryManager
{
public:
        MemoryManager(NESCart *cart);
        ~MemoryManager();

        byte ReadMemory(word offset);
        signed char ReadMemorySigned(word offset);
        void WriteMemory(word offset, byte data);
        M6502 *uCPU;
        APU   *uAPU;



        controller control1;
        controller control2;


            byte *GetMemoryHandle() { return RAM6502;}


            void DMA_transfer(word offset, byte data);
            bool disable_apu;

            NESCart *cartptr;
private:

        //shouldn't be here, but keeping it here nonetheless until later...


        byte *RAM6502;
       // byte *RAMPPU;


};

#endif // MEMORY_H
