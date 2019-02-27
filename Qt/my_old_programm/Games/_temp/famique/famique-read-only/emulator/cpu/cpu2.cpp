
/*
 *  cpu2.cpp
 *  new (and improved (tm)) cpu core
 *
 *  Created by andro on 12/15/09.
 *  Copyright 2009 sahib khadaffi (Kristofer Hammarskøld). NO RIGHTS RESERVED :-D
 *


 This 6502-implementation will strictly work on little-endian machines.
 I've overridden som basic datatypes, which might be prevalent on some platforms, so if you get a lot of compiler errors
 you probably need to #undef byte and word, in order for this shit to work.

 The CPU emulator is pretty straight forward. It uses a jumptable for each instruction and associated addressing mode.

 The CPU Emulator is not cycle accurate in the sense that it doesn't add an extra cycle if you for example cross a page boundary.
 This is fairly easy to fix, but I've been prioritizing other stuff recently, so yeah :)
 Mapper IRQs aren't supported yet either, only 'normal' IRQs are. There are LOADS of optimizations to be done, really!

 Breakdown of CPU emulation:
 1) Fetch the current opcode
 2) Load the appropriate address
 3) Execute the opcode
 4) Branch or increment PC
 4 1/2) Look up how many cycles that instruction used, and add this to internal latch variable
 5) Goto 1, ad finitum.

An external Emulator-class (or something) will have to deal with synchronization between the CPU and other periphernilia.


THE CPU EMULATOR DOESN'T SUPPORT BCD-MODE, SO THIS NEED TO BE ADDED IF YOU'D WANT TO USE THE CORE TO EMULATE C64, FOR EXAMPLE.


Stand-alone M6502 emulation is possible (and encouraged :D), you'll need to use famicom.h for datatypes, cpu2.h for class definition and cpu2.cpp for class implementation
definitions.h and definitions.cpp holds the implementation of the addressing modes and all official opcodes.
You'll need to implement your own memory manager (and NESCart ROM loader) for the platform you'd want to emulate.

*/

#include "cpu2.h"

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <string.h>

#define M6502_EXTRA_CYCLES

//#define ENABLE_TRACING
//#define ENABLE_6502_DUMMY_READS




M6502::M6502(NESCart *cart)
{
//CPU initialization
        this->SetUpTables();

        this->memory = new MemoryManager(cart);
        ppu_2 = new ppu2();
        ppu_2->SetCart(cart);

        memory->uCPU = this;
        memset(&flags, 0, sizeof(CPU_FLAGS));


#ifdef ENABLE_TRACING
        tr = fopen("/Users/andro/Desktop/6502.txt", "w");
#endif
	operand_accumulator = 0;

	A = X = Y = SP = 0;
	
	PC = 0;
        running = false;
        CPU_crash = false;


        Init();
	

	
}

M6502::~M6502()
{

//blah blah blah
    delete dis;
    delete ppu_2;
    delete memory;

}

void M6502::Init()
{
	PC = MAKE_WORD(this->memory->ReadMemory(0xFFFC), memory->ReadMemory(0xFFFD));	//reset vector in PRG
	increment = 0;
	cycles = 0;

#ifdef M6502_EXTRA_CYCLES
        extra_cycles = 0; //extra cycles handles cycles spent during page-crossings, etcetera.
#endif
//	running = true;
	branch = false;

        flags.members.unused = 1; //is always 1

        flags.members.I = 1;


        dis = new disassembler(NULL);
        //dis->disassemble(this->memory, 0x8000);
	
}

void M6502::NextOpCode()
{
/*	trace->next->PC = PC;
	trace->next->prev = trace;
	trace->next->next = new t_PC;
	trace = trace->next;

*/


    extra_cycles = 0;

    this->previous_state = this->GetCPUState();

	
	opcode = memory->ReadMemory(PC);
#ifdef ENABLE_TRACING
        this->pretrace();
#endif


	
	
	(this->*m6502_adressing[opcode])();


	(this->*m6502_mnemonics[opcode])();

#ifdef ENABLE_TRACING
        this->trace_current_instruction(increment);
	
#endif
	if(!branch) //increment the PC if we're not in a branch
		PC += increment;
	else //don't increment the PC if we're branching
	{
		branch = false; 

		// trace the program counter during branches, helps me debug this mess :-O 
/*		
			trace->PC = PC;
		trace->next = new t_PC;
		trace->next->prev = trace;
		trace = trace->next;
*/
	}

	if(operand_accumulator) //this is part of my series of ugly hacks that this version of the CPU emulator was supposed to avoid ... OHWELL :-c
		operand_accumulator = false;



        #ifdef M6502_EXTRA_CYCLES
        cycles += this->m6502_cyclecount[opcode] + extra_cycles;
        #else
        cycles += this->m6502_cyclecount[opcode];
        #endif
	adress = 0;
	value = 0;

	
}


//adressing modes

#include "definitions.cpp"

void M6502::UNIMPLEMENTED_OPCODE()
{
        PrintDebug("Unimplemented opcode!\nPC:0x%04X\nopcode:0x%02X\nCPU will now commit suicide", PC,memory->ReadMemory(PC));
        CPU_crash = true;
}

void M6502::UNIMPLEMENTED_ADRESSING_MODE()
{
        PrintDebug("Unimplemented adressing mode!\nPC:0x%04X\nopcode:0x%02X\nCPU will now commit suicide", this->PC, memory->ReadMemory(PC));
        CPU_crash = true;
}


void M6502::PrintDebug(const char *str, ...)
{
        FILE *fp = fopen("debug.txt", "a");
        char *pstr = (char *)malloc(1024); //1024 bytes ought to be enough for anyone...
	va_list args;
	va_start(args, str);
	vsprintf(pstr, str, args);
        fprintf(fp, pstr);
	printf(pstr);

	

	va_end(args);
	free(pstr);
	fclose(fp);
}







void M6502::PUSH(byte reg)
{
	
        memory->WriteMemory(((0x0100+(SP&0xff))),reg);
	SP--;
}

void M6502::Reset()
{
   // bool resume = this->isRunning();
    Halt();
    PC = MAKE_WORD(memory->ReadMemory(0xFFFC), memory->ReadMemory(0xFFFD));
    A = X = Y = SP = 0;
    memset(&this->previous_state, 0x00, sizeof(CPU_SNAPSHOT));
    memset(&this->flags, 0x00, sizeof(CPU_FLAGS));


    this->CPU_crash = false;

}

byte M6502::POP()
{
	
        byte tempSP = ((SP+1) & 0xFF); //wrapping
	byte ret_val = memory->ReadMemory( 0x0100+tempSP);
	SP++;
	return ret_val;
	
}






void M6502::SetUpTables()
{
	
	void (M6502::*m6502_mnemonics_setup[256])(void);
	void (M6502::*m6502_adressing_modes_setup[256])(void);
	
	m6502_mnemonics_setup[0x00] = &M6502::BRK;
	m6502_mnemonics_setup[0x01] = &M6502::ORA;
	m6502_mnemonics_setup[0x02] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x03] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x04] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x05] = &M6502::ORA;
	m6502_mnemonics_setup[0x06] = &M6502::ASL;
	m6502_mnemonics_setup[0x07] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x08] = &M6502::PHP;
	m6502_mnemonics_setup[0x09] = &M6502::ORA;
	m6502_mnemonics_setup[0x0A] = &M6502::ASL;
	m6502_mnemonics_setup[0x0B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x0C] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x0D] = &M6502::ORA;
	m6502_mnemonics_setup[0x0E] = &M6502::ASL;
	m6502_mnemonics_setup[0x0F] = &M6502::UNIMPLEMENTED_OPCODE;

	m6502_mnemonics_setup[0x10] = &M6502::BPL;
	m6502_mnemonics_setup[0x11] = &M6502::ORA;
	m6502_mnemonics_setup[0x12] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x13] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x14] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x15] = &M6502::ORA;
	m6502_mnemonics_setup[0x16] = &M6502::ASL;
	m6502_mnemonics_setup[0x17] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x18] = &M6502::CLC;
	m6502_mnemonics_setup[0x19] = &M6502::ORA;
	m6502_mnemonics_setup[0x1A] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x1B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x1C] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x1D] = &M6502::ORA;
	m6502_mnemonics_setup[0x1E] = &M6502::ASL;

	m6502_mnemonics_setup[0x1F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x20] = &M6502::JSR;
	m6502_mnemonics_setup[0x21] = &M6502::AND;
	m6502_mnemonics_setup[0x22] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x23] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x24] = &M6502::BIT;
	m6502_mnemonics_setup[0x25] = &M6502::AND;
	m6502_mnemonics_setup[0x26] = &M6502::ROL;
	m6502_mnemonics_setup[0x27] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x28] = &M6502::PLP;
	m6502_mnemonics_setup[0x29] = &M6502::AND;
	m6502_mnemonics_setup[0x2A] = &M6502::ROL;
	m6502_mnemonics_setup[0x2B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x2C] = &M6502::BIT;
	m6502_mnemonics_setup[0x2D] = &M6502::AND;
	m6502_mnemonics_setup[0x2E] = &M6502::ROL;
	m6502_mnemonics_setup[0x2F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x30] = &M6502::BMI;
	m6502_mnemonics_setup[0x31] = &M6502::AND;
	m6502_mnemonics_setup[0x32] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x33] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x34] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x35] = &M6502::AND;
	m6502_mnemonics_setup[0x36] = &M6502::ROL;
	m6502_mnemonics_setup[0x37] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x38] = &M6502::SEC;
	m6502_mnemonics_setup[0x39] = &M6502::AND;
	m6502_mnemonics_setup[0x3A] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x3B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x3C] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x3D] = &M6502::AND;
	m6502_mnemonics_setup[0x3E] = &M6502::ROL;
	m6502_mnemonics_setup[0x3F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x40] = &M6502::RTI;
	m6502_mnemonics_setup[0x41] = &M6502::EOR;
	m6502_mnemonics_setup[0x42] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x43] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x44] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x45] = &M6502::EOR;
	m6502_mnemonics_setup[0x46] = &M6502::LSR;
	m6502_mnemonics_setup[0x47] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x48] = &M6502::PHA;
	m6502_mnemonics_setup[0x49] = &M6502::EOR;
	m6502_mnemonics_setup[0x4A] = &M6502::LSR;
	m6502_mnemonics_setup[0x4B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x4C] = &M6502::JMP;
	m6502_mnemonics_setup[0x4D] = &M6502::EOR;
	m6502_mnemonics_setup[0x4E] = &M6502::LSR;
	m6502_mnemonics_setup[0x4F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x50] = &M6502::BVC;
	m6502_mnemonics_setup[0x51] = &M6502::EOR;
	m6502_mnemonics_setup[0x52] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x53] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x54] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x55] = &M6502::EOR;
	m6502_mnemonics_setup[0x56] = &M6502::LSR;
	m6502_mnemonics_setup[0x57] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x58] = &M6502::CLI;
	m6502_mnemonics_setup[0x59] = &M6502::EOR;
	m6502_mnemonics_setup[0x5A] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x5B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x5C] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x5D] = &M6502::EOR;
	m6502_mnemonics_setup[0x5E] = &M6502::LSR;
	m6502_mnemonics_setup[0x5F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x60] = &M6502::RTS;
	m6502_mnemonics_setup[0x61] = &M6502::ADC;
	m6502_mnemonics_setup[0x62] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x63] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x64] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x65] = &M6502::ADC;
	m6502_mnemonics_setup[0x66] = &M6502::ROR;
	m6502_mnemonics_setup[0x67] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x68] = &M6502::PLA;
	m6502_mnemonics_setup[0x69] = &M6502::ADC;
	m6502_mnemonics_setup[0x6A] = &M6502::ROR;
	m6502_mnemonics_setup[0x6B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x6C] = &M6502::JMP;
	m6502_mnemonics_setup[0x6D] = &M6502::ADC;
	m6502_mnemonics_setup[0x6E] = &M6502::ROR;
	m6502_mnemonics_setup[0x6F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x70] = &M6502::BVS;
	m6502_mnemonics_setup[0x71] = &M6502::ADC;
	m6502_mnemonics_setup[0x72] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x73] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x74] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x75] = &M6502::ADC;
	m6502_mnemonics_setup[0x76] = &M6502::ROR;
	m6502_mnemonics_setup[0x77] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x78] = &M6502::SEI;
	m6502_mnemonics_setup[0x79] = &M6502::ADC;
	m6502_mnemonics_setup[0x7A] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x7B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x7C] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x7D] = &M6502::ADC;
	m6502_mnemonics_setup[0x7E] = &M6502::ROR;
	m6502_mnemonics_setup[0x7F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x80] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x81] = &M6502::STA;
	m6502_mnemonics_setup[0x82] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x83] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x84] = &M6502::STY;
	m6502_mnemonics_setup[0x85] = &M6502::STA;
	m6502_mnemonics_setup[0x86] = &M6502::STX;
	m6502_mnemonics_setup[0x87] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x88] = &M6502::DEY;
	m6502_mnemonics_setup[0x89] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x8A] = &M6502::TXA;
	m6502_mnemonics_setup[0x8B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x8C] = &M6502::STY;
	m6502_mnemonics_setup[0x8D] = &M6502::STA;
	m6502_mnemonics_setup[0x8E] = &M6502::STX;
	m6502_mnemonics_setup[0x8F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x90] = &M6502::BCC;
	m6502_mnemonics_setup[0x91] = &M6502::STA;
	m6502_mnemonics_setup[0x92] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x93] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x94] = &M6502::STY;
	m6502_mnemonics_setup[0x95] = &M6502::STA;
	m6502_mnemonics_setup[0x96] = &M6502::STX;
	m6502_mnemonics_setup[0x97] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x98] = &M6502::TYA;
	m6502_mnemonics_setup[0x99] = &M6502::STA;
	m6502_mnemonics_setup[0x9A] = &M6502::TXS;
	m6502_mnemonics_setup[0x9B] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x9C] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x9D] = &M6502::STA;
	m6502_mnemonics_setup[0x9E] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0x9F] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xA0] = &M6502::LDY;
	m6502_mnemonics_setup[0xA1] = &M6502::LDA;
	m6502_mnemonics_setup[0xA2] = &M6502::LDX;
	m6502_mnemonics_setup[0xA3] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xA4] = &M6502::LDY;
	m6502_mnemonics_setup[0xA5] = &M6502::LDA;
	m6502_mnemonics_setup[0xA6] = &M6502::LDX;
	m6502_mnemonics_setup[0xA7] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xA8] = &M6502::TAY;
	m6502_mnemonics_setup[0xA9] = &M6502::LDA;
	m6502_mnemonics_setup[0xAA] = &M6502::TAX;
	m6502_mnemonics_setup[0xAB] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xAC] = &M6502::LDY;
	m6502_mnemonics_setup[0xAD] = &M6502::LDA;
	m6502_mnemonics_setup[0xAE] = &M6502::LDX;
	m6502_mnemonics_setup[0xAF] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xB0] = &M6502::BCS;
	m6502_mnemonics_setup[0xB1] = &M6502::LDA;
	m6502_mnemonics_setup[0xB2] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xB3] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xB4] = &M6502::LDY;
	m6502_mnemonics_setup[0xB5] = &M6502::LDA;
	m6502_mnemonics_setup[0xB6] = &M6502::LDX;
	m6502_mnemonics_setup[0xB7] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xB8] = &M6502::CLV;
	m6502_mnemonics_setup[0xB9] = &M6502::LDA;
	m6502_mnemonics_setup[0xBA] = &M6502::TSX;
	m6502_mnemonics_setup[0xBB] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xBC] = &M6502::LDY;
	m6502_mnemonics_setup[0xBD] = &M6502::LDA;
	m6502_mnemonics_setup[0xBE] = &M6502::LDX;
	m6502_mnemonics_setup[0xBF] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xC0] = &M6502::CPY;
	m6502_mnemonics_setup[0xC1] = &M6502::CMP;
	m6502_mnemonics_setup[0xC2] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xC3] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xC4] = &M6502::CPY;
	m6502_mnemonics_setup[0xC5] = &M6502::CMP;
	m6502_mnemonics_setup[0xC6] = &M6502::DEC;
	m6502_mnemonics_setup[0xC7] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xC8] = &M6502::INY;
	m6502_mnemonics_setup[0xC9] = &M6502::CMP;
	m6502_mnemonics_setup[0xCA] = &M6502::DEX;
	m6502_mnemonics_setup[0xCB] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xCC] = &M6502::CPY;
	m6502_mnemonics_setup[0xCD] = &M6502::CMP;
	m6502_mnemonics_setup[0xCE] = &M6502::DEC;
	m6502_mnemonics_setup[0xCF] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xD0] = &M6502::BNE;
	m6502_mnemonics_setup[0xD1] = &M6502::CMP;
	m6502_mnemonics_setup[0xD2] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xD3] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xD4] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xD5] = &M6502::CMP;
	m6502_mnemonics_setup[0xD6] = &M6502::DEC;
	m6502_mnemonics_setup[0xD7] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xD8] = &M6502::CLD;
	m6502_mnemonics_setup[0xD9] = &M6502::CMP;
	m6502_mnemonics_setup[0xDA] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xDB] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xDC] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xDD] = &M6502::CMP;
	m6502_mnemonics_setup[0xDE] = &M6502::DEC;
	m6502_mnemonics_setup[0xDF] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xE0] = &M6502::CPX;
	m6502_mnemonics_setup[0xE1] = &M6502::SBC;
	m6502_mnemonics_setup[0xE2] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xE3] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xE4] = &M6502::CPX;
	m6502_mnemonics_setup[0xE5] = &M6502::SBC;
	m6502_mnemonics_setup[0xE6] = &M6502::INC;
	m6502_mnemonics_setup[0xE7] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xE8] = &M6502::INX;
	m6502_mnemonics_setup[0xE9] = &M6502::SBC;
	m6502_mnemonics_setup[0xEA] = &M6502::NOP;
	m6502_mnemonics_setup[0xEB] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xEC] = &M6502::CPX;
	m6502_mnemonics_setup[0xED] = &M6502::SBC;
	m6502_mnemonics_setup[0xEE] = &M6502::INC;
	m6502_mnemonics_setup[0xEF] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xF0] = &M6502::BEQ;
	m6502_mnemonics_setup[0xF1] = &M6502::SBC;
	m6502_mnemonics_setup[0xF2] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xF3] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xF4] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xF5] = &M6502::SBC;
	m6502_mnemonics_setup[0xF6] = &M6502::INC;
	m6502_mnemonics_setup[0xF7] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xF8] = &M6502::SED;
	m6502_mnemonics_setup[0xF9] = &M6502::SBC;
	m6502_mnemonics_setup[0xFA] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xFB] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xFC] = &M6502::UNIMPLEMENTED_OPCODE;
	m6502_mnemonics_setup[0xFD] = &M6502::SBC;
	m6502_mnemonics_setup[0xFE] = &M6502::INC;
	m6502_mnemonics_setup[0xFF] = &M6502::UNIMPLEMENTED_OPCODE;


	m6502_adressing_modes_setup[0x00] = &M6502::implied;
	m6502_adressing_modes_setup[0x01] = &M6502::indexedX;
	m6502_adressing_modes_setup[0x02] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x03] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x04] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x05] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x06] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x07] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x08] = &M6502::implied;
	m6502_adressing_modes_setup[0x09] = &M6502::immediate;
	m6502_adressing_modes_setup[0x0A] = &M6502::accumulator;
	m6502_adressing_modes_setup[0x0B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x0C] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x0D] = &M6502::absolute;
	m6502_adressing_modes_setup[0x0E] = &M6502::absolute;
	m6502_adressing_modes_setup[0x0F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x10] = &M6502::relative;
	m6502_adressing_modes_setup[0x11] = &M6502::indirectY;
	m6502_adressing_modes_setup[0x12] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x13] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x14] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x15] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x16] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x17] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x18] = &M6502::implied;
	m6502_adressing_modes_setup[0x19] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0x1A] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x1B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x1C] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x1D] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x1E] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x1F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x20] = &M6502::absolute;
	m6502_adressing_modes_setup[0x21] = &M6502::indexedX;
	m6502_adressing_modes_setup[0x22] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x23] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x24] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x25] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x26] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x27] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x28] = &M6502::implied;
	m6502_adressing_modes_setup[0x29] = &M6502::immediate;
	m6502_adressing_modes_setup[0x2A] = &M6502::accumulator;
	m6502_adressing_modes_setup[0x2B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x2C] = &M6502::absolute;
	m6502_adressing_modes_setup[0x2D] = &M6502::absolute;
	m6502_adressing_modes_setup[0x2E] = &M6502::absolute;
	m6502_adressing_modes_setup[0x2F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x30] = &M6502::relative;
	m6502_adressing_modes_setup[0x31] = &M6502::indirectY;
	m6502_adressing_modes_setup[0x32] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x33] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x34] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x35] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x36] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x37] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x38] = &M6502::implied;
	m6502_adressing_modes_setup[0x39] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0x3A] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x3B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x3C] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x3D] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x3E] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x3F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x40] = &M6502::implied;
	m6502_adressing_modes_setup[0x41] = &M6502::indexedX;
	m6502_adressing_modes_setup[0x42] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x43] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x44] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x45] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x46] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x47] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x48] = &M6502::implied;
	m6502_adressing_modes_setup[0x49] = &M6502::immediate;
	m6502_adressing_modes_setup[0x4A] = &M6502::accumulator;
	m6502_adressing_modes_setup[0x4B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x4C] = &M6502::absolute;
	m6502_adressing_modes_setup[0x4D] = &M6502::absolute;
	m6502_adressing_modes_setup[0x4E] = &M6502::absolute;
	m6502_adressing_modes_setup[0x4F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x50] = &M6502::relative;
	m6502_adressing_modes_setup[0x51] = &M6502::indirectY;
	m6502_adressing_modes_setup[0x52] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x53] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x54] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x55] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x56] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x57] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x58] = &M6502::implied;
	m6502_adressing_modes_setup[0x59] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0x5A] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x5B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x5C] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x5D] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x5E] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x5F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x60] = &M6502::implied;
	m6502_adressing_modes_setup[0x61] = &M6502::indexedX;
	m6502_adressing_modes_setup[0x62] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x63] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x64] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x65] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x66] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x67] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x68] = &M6502::implied;
	m6502_adressing_modes_setup[0x69] = &M6502::immediate;
	m6502_adressing_modes_setup[0x6A] = &M6502::accumulator;
	m6502_adressing_modes_setup[0x6B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x6C] = &M6502::indirect;
	m6502_adressing_modes_setup[0x6D] = &M6502::absolute;
	m6502_adressing_modes_setup[0x6E] = &M6502::absolute;
	m6502_adressing_modes_setup[0x6F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x70] = &M6502::relative;
	m6502_adressing_modes_setup[0x71] = &M6502::indirectY;
	m6502_adressing_modes_setup[0x72] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x73] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x74] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x75] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x76] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x77] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x78] = &M6502::implied;
	m6502_adressing_modes_setup[0x79] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0x7A] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x7B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x7C] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x7D] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x7E] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x7F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x80] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x81] = &M6502::indexedX;
	m6502_adressing_modes_setup[0x82] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x83] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x84] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x85] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x86] = &M6502::zeropage;
	m6502_adressing_modes_setup[0x87] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x88] = &M6502::implied;
	m6502_adressing_modes_setup[0x89] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x8A] = &M6502::implied;
	m6502_adressing_modes_setup[0x8B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x8C] = &M6502::absolute;
	m6502_adressing_modes_setup[0x8D] = &M6502::absolute;
	m6502_adressing_modes_setup[0x8E] = &M6502::absolute;
	m6502_adressing_modes_setup[0x8F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x90] = &M6502::relative;
	m6502_adressing_modes_setup[0x91] = &M6502::indirectY;
	m6502_adressing_modes_setup[0x92] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x93] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x94] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x95] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0x96] = &M6502::zeropageY;
	m6502_adressing_modes_setup[0x97] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x98] = &M6502::implied;
	m6502_adressing_modes_setup[0x99] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0x9A] = &M6502::implied;
	m6502_adressing_modes_setup[0x9B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x9C] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x9D] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0x9E] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0x9F] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xA0] = &M6502::immediate;
	m6502_adressing_modes_setup[0xA1] = &M6502::indexedX;
	m6502_adressing_modes_setup[0xA2] = &M6502::immediate;
	m6502_adressing_modes_setup[0xA3] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xA4] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xA5] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xA6] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xA7] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xA8] = &M6502::implied;
	m6502_adressing_modes_setup[0xA9] = &M6502::immediate;
	m6502_adressing_modes_setup[0xAA] = &M6502::implied;
	m6502_adressing_modes_setup[0xAB] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xAC] = &M6502::absolute;
	m6502_adressing_modes_setup[0xAD] = &M6502::absolute;
	m6502_adressing_modes_setup[0xAE] = &M6502::absolute;
	m6502_adressing_modes_setup[0xAF] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xB0] = &M6502::relative;
	m6502_adressing_modes_setup[0xB1] = &M6502::indirectY;
	m6502_adressing_modes_setup[0xB2] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xB3] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xB4] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0xB5] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0xB6] = &M6502::zeropageY;
	m6502_adressing_modes_setup[0xB7] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xB8] = &M6502::implied;
	m6502_adressing_modes_setup[0xB9] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0xBA] = &M6502::implied;
	m6502_adressing_modes_setup[0xBB] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xBC] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0xBD] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0xBE] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0xBF] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xC0] = &M6502::immediate;
	m6502_adressing_modes_setup[0xC1] = &M6502::indexedX;
	m6502_adressing_modes_setup[0xC2] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xC3] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xC4] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xC5] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xC6] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xC7] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xC8] = &M6502::implied;
	m6502_adressing_modes_setup[0xC9] = &M6502::immediate;
	m6502_adressing_modes_setup[0xCA] = &M6502::implied;
	m6502_adressing_modes_setup[0xCB] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xCC] = &M6502::absolute;
	m6502_adressing_modes_setup[0xCD] = &M6502::absolute;
	m6502_adressing_modes_setup[0xCE] = &M6502::absolute;
	m6502_adressing_modes_setup[0xCF] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xD0] = &M6502::relative;
	m6502_adressing_modes_setup[0xD1] = &M6502::indirectY;
	m6502_adressing_modes_setup[0xD2] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xD3] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xD4] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xD5] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0xD6] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0xD7] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xD8] = &M6502::implied;
	m6502_adressing_modes_setup[0xD9] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0xDA] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xDB] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xDC] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xDD] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0xDE] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0xDF] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xE0] = &M6502::immediate;
	m6502_adressing_modes_setup[0xE1] = &M6502::indexedX;
	m6502_adressing_modes_setup[0xE2] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xE3] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xE4] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xE5] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xE6] = &M6502::zeropage;
	m6502_adressing_modes_setup[0xE7] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xE8] = &M6502::implied;
	m6502_adressing_modes_setup[0xE9] = &M6502::immediate;
	m6502_adressing_modes_setup[0xEA] = &M6502::implied;
	m6502_adressing_modes_setup[0xEB] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xEC] = &M6502::absolute;
	m6502_adressing_modes_setup[0xED] = &M6502::absolute;
	m6502_adressing_modes_setup[0xEE] = &M6502::absolute;
	m6502_adressing_modes_setup[0xEF] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xF0] = &M6502::relative;
	m6502_adressing_modes_setup[0xF1] = &M6502::indirectY;
	m6502_adressing_modes_setup[0xF2] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xF3] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xF4] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xF5] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0xF6] = &M6502::zeropageX;
	m6502_adressing_modes_setup[0xF7] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xF8] = &M6502::implied;
	m6502_adressing_modes_setup[0xF9] = &M6502::absoluteY;
	m6502_adressing_modes_setup[0xFA] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xFB] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xFC] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
	m6502_adressing_modes_setup[0xFD] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0xFE] = &M6502::absoluteX;
	m6502_adressing_modes_setup[0xFF] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;




	m6502_cyclecount[0x00]=7;
	m6502_cyclecount[0x01]=6;
	m6502_cyclecount[0x02]=0;
	m6502_cyclecount[0x03]=0;
	m6502_cyclecount[0x04]=0;
	m6502_cyclecount[0x05]=2;
	m6502_cyclecount[0x06]=5;
	m6502_cyclecount[0x07]=0;
	m6502_cyclecount[0x08]=3;
	m6502_cyclecount[0x09]=2;
	m6502_cyclecount[0x0A]=2;
	m6502_cyclecount[0x0B]=0;
	m6502_cyclecount[0x0C]=0;
	m6502_cyclecount[0x0D]=4;
	m6502_cyclecount[0x0E]=6;
	m6502_cyclecount[0x0F]=0;
	m6502_cyclecount[0x10]=2;
	m6502_cyclecount[0x11]=5;
	m6502_cyclecount[0x12]=0;
	m6502_cyclecount[0x13]=0;
	m6502_cyclecount[0x14]=0;
	m6502_cyclecount[0x15]=3;
	m6502_cyclecount[0x16]=6;
	m6502_cyclecount[0x17]=0;
	m6502_cyclecount[0x18]=2;
	m6502_cyclecount[0x19]=4;
	m6502_cyclecount[0x1A]=0;
	m6502_cyclecount[0x1B]=0;
	m6502_cyclecount[0x1C]=0;
	m6502_cyclecount[0x1D]=4;
	m6502_cyclecount[0x1E]=7;
	m6502_cyclecount[0x1F]=0;
	m6502_cyclecount[0x20]=6;
	m6502_cyclecount[0x21]=6;
	m6502_cyclecount[0x22]=0;
	m6502_cyclecount[0x23]=0;
	m6502_cyclecount[0x24]=3;
	m6502_cyclecount[0x25]=2;
	m6502_cyclecount[0x26]=5;
	m6502_cyclecount[0x27]=0;
	m6502_cyclecount[0x28]=4;
	m6502_cyclecount[0x29]=2;
	m6502_cyclecount[0x2A]=2;
	m6502_cyclecount[0x2B]=0;
	m6502_cyclecount[0x2C]=4;
	m6502_cyclecount[0x2D]=4;
	m6502_cyclecount[0x2E]=6;
	m6502_cyclecount[0x2F]=0;
	m6502_cyclecount[0x30]=2;
	m6502_cyclecount[0x31]=5;
	m6502_cyclecount[0x32]=0;
	m6502_cyclecount[0x33]=0;
	m6502_cyclecount[0x34]=0;
	m6502_cyclecount[0x35]=3;
	m6502_cyclecount[0x36]=6;
	m6502_cyclecount[0x37]=0;
	m6502_cyclecount[0x38]=2;
	m6502_cyclecount[0x39]=4;
	m6502_cyclecount[0x3A]=0;
	m6502_cyclecount[0x3B]=0;
	m6502_cyclecount[0x3C]=0;
	m6502_cyclecount[0x3D]=4;
	m6502_cyclecount[0x3E]=7;
	m6502_cyclecount[0x3F]=0;
	m6502_cyclecount[0x40]=6;
	m6502_cyclecount[0x41]=6;
	m6502_cyclecount[0x42]=0;
	m6502_cyclecount[0x43]=0;
	m6502_cyclecount[0x44]=0;
	m6502_cyclecount[0x45]=3;
	m6502_cyclecount[0x46]=5;
	m6502_cyclecount[0x47]=0;
	m6502_cyclecount[0x48]=3;
	m6502_cyclecount[0x49]=2;
	m6502_cyclecount[0x4A]=2;
	m6502_cyclecount[0x4B]=0;
	m6502_cyclecount[0x4C]=3;
	m6502_cyclecount[0x4D]=4;
	m6502_cyclecount[0x4E]=6;
	m6502_cyclecount[0x4F]=0;
	m6502_cyclecount[0x50]=2;
	m6502_cyclecount[0x51]=5;
	m6502_cyclecount[0x52]=0;
	m6502_cyclecount[0x53]=0;
	m6502_cyclecount[0x54]=0;
	m6502_cyclecount[0x55]=4;
	m6502_cyclecount[0x56]=6;
	m6502_cyclecount[0x57]=0;
	m6502_cyclecount[0x58]=2;
	m6502_cyclecount[0x59]=4;
	m6502_cyclecount[0x5A]=0;
	m6502_cyclecount[0x5B]=0;
	m6502_cyclecount[0x5C]=0;
	m6502_cyclecount[0x5D]=4;
	m6502_cyclecount[0x5E]=7;
	m6502_cyclecount[0x5F]=0;
	m6502_cyclecount[0x60]=6;
	m6502_cyclecount[0x61]=6;
	m6502_cyclecount[0x62]=0;
	m6502_cyclecount[0x63]=0;
	m6502_cyclecount[0x64]=0;
	m6502_cyclecount[0x65]=3;
	m6502_cyclecount[0x66]=5;
	m6502_cyclecount[0x67]=0;
	m6502_cyclecount[0x68]=4;
	m6502_cyclecount[0x69]=2;
	m6502_cyclecount[0x6A]=2;
	m6502_cyclecount[0x6B]=0;
	m6502_cyclecount[0x6C]=5;
	m6502_cyclecount[0x6D]=4;
	m6502_cyclecount[0x6E]=6;
	m6502_cyclecount[0x6F]=0;
	m6502_cyclecount[0x70]=2;
	m6502_cyclecount[0x71]=5;
	m6502_cyclecount[0x72]=0;
	m6502_cyclecount[0x73]=0;
	m6502_cyclecount[0x74]=0;
	m6502_cyclecount[0x75]=4;
	m6502_cyclecount[0x76]=6;
	m6502_cyclecount[0x77]=0;
	m6502_cyclecount[0x78]=2;
	m6502_cyclecount[0x79]=4;
	m6502_cyclecount[0x7A]=0;
	m6502_cyclecount[0x7B]=0;
	m6502_cyclecount[0x7C]=0;
	m6502_cyclecount[0x7D]=4;
	m6502_cyclecount[0x7E]=7;
	m6502_cyclecount[0x7F]=0;
	m6502_cyclecount[0x80]=0;
	m6502_cyclecount[0x81]=6;
	m6502_cyclecount[0x82]=0;
	m6502_cyclecount[0x83]=0;
	m6502_cyclecount[0x84]=3;
	m6502_cyclecount[0x85]=3;
	m6502_cyclecount[0x86]=3;
	m6502_cyclecount[0x87]=0;
	m6502_cyclecount[0x88]=2;
	m6502_cyclecount[0x89]=0;
	m6502_cyclecount[0x8A]=2;
	m6502_cyclecount[0x8B]=0;
	m6502_cyclecount[0x8C]=4;
	m6502_cyclecount[0x8D]=4;
	m6502_cyclecount[0x8E]=4;
	m6502_cyclecount[0x8F]=0;
	m6502_cyclecount[0x90]=2;
	m6502_cyclecount[0x91]=6;
	m6502_cyclecount[0x92]=0;
	m6502_cyclecount[0x93]=0;
	m6502_cyclecount[0x94]=4;
	m6502_cyclecount[0x95]=4;
	m6502_cyclecount[0x96]=4;
	m6502_cyclecount[0x97]=0;
	m6502_cyclecount[0x98]=2;
	m6502_cyclecount[0x99]=5;
	m6502_cyclecount[0x9A]=2;
	m6502_cyclecount[0x9B]=0;
	m6502_cyclecount[0x9C]=0;
	m6502_cyclecount[0x9D]=5;
	m6502_cyclecount[0x9E]=0;
	m6502_cyclecount[0x9F]=0;
	m6502_cyclecount[0xA0]=2;
	m6502_cyclecount[0xA1]=6;
	m6502_cyclecount[0xA2]=2;
	m6502_cyclecount[0xA3]=0;
	m6502_cyclecount[0xA4]=3;
	m6502_cyclecount[0xA5]=3;
	m6502_cyclecount[0xA6]=3;
	m6502_cyclecount[0xA7]=0;
	m6502_cyclecount[0xA8]=2;
	m6502_cyclecount[0xA9]=2;
	m6502_cyclecount[0xAA]=2;
	m6502_cyclecount[0xAB]=0;
	m6502_cyclecount[0xAC]=4;
	m6502_cyclecount[0xAD]=4;
	m6502_cyclecount[0xAE]=4;
	m6502_cyclecount[0xAF]=0;
	m6502_cyclecount[0xB0]=2;
	m6502_cyclecount[0xB1]=5;
	m6502_cyclecount[0xB2]=0;
	m6502_cyclecount[0xB3]=0;
	m6502_cyclecount[0xB4]=4;
	m6502_cyclecount[0xB5]=4;
	m6502_cyclecount[0xB6]=4;
	m6502_cyclecount[0xB7]=0;
	m6502_cyclecount[0xB8]=2;
	m6502_cyclecount[0xB9]=4;
	m6502_cyclecount[0xBA]=2;
	m6502_cyclecount[0xBB]=0;
	m6502_cyclecount[0xBC]=4;
	m6502_cyclecount[0xBD]=4;
	m6502_cyclecount[0xBE]=4;
	m6502_cyclecount[0xBF]=0;
	m6502_cyclecount[0xC0]=2;
	m6502_cyclecount[0xC1]=6;
	m6502_cyclecount[0xC2]=0;
	m6502_cyclecount[0xC3]=0;
	m6502_cyclecount[0xC4]=3;
	m6502_cyclecount[0xC5]=3;
	m6502_cyclecount[0xC6]=5;
	m6502_cyclecount[0xC7]=0;
	m6502_cyclecount[0xC8]=2;
	m6502_cyclecount[0xC9]=2;
	m6502_cyclecount[0xCA]=2;
	m6502_cyclecount[0xCB]=0;
	m6502_cyclecount[0xCC]=4;
	m6502_cyclecount[0xCD]=4;
	m6502_cyclecount[0xCE]=6;
	m6502_cyclecount[0xCF]=0;
	m6502_cyclecount[0xD0]=2;
	m6502_cyclecount[0xD1]=5;
	m6502_cyclecount[0xD2]=0;
	m6502_cyclecount[0xD3]=0;
	m6502_cyclecount[0xD4]=0;
	m6502_cyclecount[0xD5]=4;
	m6502_cyclecount[0xD6]=6;
	m6502_cyclecount[0xD7]=0;
	m6502_cyclecount[0xD8]=2;
	m6502_cyclecount[0xD9]=4;
	m6502_cyclecount[0xDA]=0;
	m6502_cyclecount[0xDB]=0;
	m6502_cyclecount[0xDC]=0;
	m6502_cyclecount[0xDD]=4;
	m6502_cyclecount[0xDE]=7;
	m6502_cyclecount[0xDF]=0;
	m6502_cyclecount[0xE0]=2;
	m6502_cyclecount[0xE1]=6;
	m6502_cyclecount[0xE2]=0;
	m6502_cyclecount[0xE3]=0;
	m6502_cyclecount[0xE4]=3;
	m6502_cyclecount[0xE5]=3;
	m6502_cyclecount[0xE6]=5;
	m6502_cyclecount[0xE7]=0;
	m6502_cyclecount[0xE8]=2;
	m6502_cyclecount[0xE9]=2;
	m6502_cyclecount[0xEA]=2;
	m6502_cyclecount[0xEB]=0;
	m6502_cyclecount[0xEC]=4;
	m6502_cyclecount[0xED]=4;
	m6502_cyclecount[0xEE]=6;
	m6502_cyclecount[0xEF]=0;
	m6502_cyclecount[0xF0]=2;
	m6502_cyclecount[0xF1]=5;
	m6502_cyclecount[0xF2]=0;
	m6502_cyclecount[0xF3]=0;
	m6502_cyclecount[0xF4]=0;
	m6502_cyclecount[0xF5]=4;
	m6502_cyclecount[0xF6]=6;
	m6502_cyclecount[0xF7]=0;
	m6502_cyclecount[0xF8]=2;
	m6502_cyclecount[0xF9]=4;
	m6502_cyclecount[0xFA]=0;
	m6502_cyclecount[0xFB]=0;
	m6502_cyclecount[0xFC]=0;
	m6502_cyclecount[0xFD]=4;
	m6502_cyclecount[0xFE]=7;
	m6502_cyclecount[0xFF]=0;

/*
	char opcodes[256][4] = {
      "BRK", "ORA", "BAD", "BAD", "BAD", "ORA", "ASL", "BAD",
      "PHP", "ORA", "ASL", "BAD", "BAD", "ORA", "ASL", "BAD",
      "BPL", "ORA", "BAD", "BAD", "BAD", "ORA", "ASL", "BAD",
      "CLC", "ORA", "BAD", "BAD", "BAD", "ORA", "ASL", "BAD",
      "JSR", "AND", "BAD", "BAD", "BIT", "AND", "ROL", "BAD",
      "PLP", "AND", "ROL", "BAD", "BIT", "AND", "ROL", "BAD",
      "BMI", "AND", "BAD", "BAD", "BAD", "AND", "ROL", "BAD",
      "SEC", "AND", "BAD", "BAD", "BAD", "AND", "ROL", "BAD",
      "RTI", "EOR", "BAD", "BAD", "BAD", "EOR", "LSR", "BAD",
      "PHA", "EOR", "LSR", "BAD", "JMP", "EOR", "LSR", "BAD",
      "BVC", "EOR", "BAD", "BAD", "BAD", "EOR", "LSR", "BAD",
      "CLI", "EOR", "BAD", "BAD", "BAD", "EOR", "LSR", "BAD",
      "RTS", "ADC", "BAD", "BAD", "BAD", "ADC", "ROR", "BAD",
      "PLA", "ADC", "ROR", "BAD", "JMP", "ADC", "ROR", "BAD",
      "BVS", "ADC", "BAD", "BAD", "BAD", "ADC", "ROR", "BAD",
      "SEI", "ADC", "BAD", "BAD", "BAD", "ADC", "ROR", "BAD",
      "BAD", "STA", "BAD", "BAD", "STY", "STA", "STX", "BAD",
      "DEY", "BAD", "TXA", "BAD", "STY", "STA", "STX", "BAD",
      "BCC", "STA", "BAD", "BAD", "STY", "STA", "STX", "BAD",
      "TYA", "STA", "TXS", "BAD", "BAD", "STA", "BAD", "BAD",
      "LDY", "LDA", "LDX", "BAD", "LDY", "LDA", "LDX", "BAD",
      "TAY", "LDA", "TAX", "BAD", "LDY", "LDA", "LDX", "BAD",
      "BCS", "LDA", "BAD", "BAD", "LDY", "LDA", "LDX", "BAD",
      "CLV", "LDA", "TSX", "BAD", "LDY", "LDA", "LDX", "BAD",
      "CPY", "CMP", "BAD", "BAD", "CPY", "CMP", "DEC", "BAD",
      "INY", "CMP", "DEX", "BAD", "CPY", "CMP", "DEC", "BAD",
      "BNE", "CMP", "BAD", "BAD", "BAD", "CMP", "DEC", "BAD",
      "CLD", "CMP", "BAD", "BAD", "BAD", "CMP", "DEC", "BAD",
      "CPX", "SBC", "BAD", "BAD", "CPX", "SBC", "INC", "BAD",
      "INX", "SBC", "NOP", "BAD", "CPX", "SBC", "INC", "BAD",
      "BEQ", "SBC", "BAD", "BAD", "BAD", "SBC", "INC", "BAD",
      "SED", "SBC", "BAD", "BAD", "BAD", "SBC", "INC", "BAD"};


	  memcpy(&mnemonics_txt ,&opcodes, 256*4);
	
*/
	memcpy(m6502_mnemonics,m6502_mnemonics_setup, sizeof(m6502_mnemonics_setup));	
	memcpy(m6502_adressing, m6502_adressing_modes_setup, sizeof(m6502_mnemonics_setup));
	

	
	
}

CPU_SNAPSHOT M6502::GetCPUState()
{
    CPU_SNAPSHOT snap;
    snap.pc = PC;
    snap.a = A;
    snap.x = X;
    snap.y = Y;
    snap.fl = flags;
    snap.SP = SP;
    snap.next_opcode = memory->ReadMemory(PC);
    snap.current_opcode = this->opcode;
    return snap;
}

void M6502::check_extra_cycles_abs_x(unsigned short address, unsigned char opcode)
{

    switch(opcode)
    {


case 0x7d: //ADC Abs,x add one for page-cross
case 0x3d: //AND abs, x
case 0xDD: //CMP abs, x
case 0x5D: //EOR abs, x
case 0xBD: //LDA abs, x
case 0x1D: //ORA abs, x
case 0xBC: //LDY abs,x
    if( (((address << 2) + X) > 0xFF))
        this->extra_cycles = 1;
    return;
    default:break;
}
}
void M6502::check_extra_cycles_abs_y(unsigned short address, unsigned char opcode)
{
    switch(opcode)
    {
    case 0x79: //ADC abs,y
    case 0x39: //AND abs, y

    case 0xD9: //CMP abs, y
    case 0x59: //EOR abs, y
    case 0xB9: //LDA abs, y
    case 0x19: //ORA abs, y
    case 0xF9: //SBC abs, y

    if(  (((address << 2 ) + Y) > 0xFF))
           this->extra_cycles = 1;
    return;
    default: break;
    }
}

void M6502::check_extra_cycles_ind_y(unsigned short address, unsigned char opcode)
{
    switch(opcode)
    {

    case 0x71: //ADC ind, y

    case 0x31: //AND ind, y

    case 0xD1: //cmp ind,y

    case 0x51: //EOR ind, y

    case 0xB1: //LDA ind, y


    case 0x11: //ORA ind, y

    case 0xF1: //SBC ind, y

        if( ((address << 2) + Y) > 0xFF)
            this->extra_cycles = 1;
        return;
    default: break;



    }
}

void M6502::check_extra_cycles_branching(unsigned short address, signed short branch_offset )
{


    switch(opcode)
    {


        //branch instructions

        //+one cycle if branch succeeds, +2 to new page.
    case 0x90: //BCC
    case 0xb0: //BCS
    case 0xF0: //BEQ
    case 0x30: //BMI
    case 0xD0: //BEQ
    case 0x10: //BPL
    case 0x50: //BVC
    case 0x70: //BVS
        if(branch_offset > 0)
        {
            if( ((address<<2) + branch_offset) > 0xFF)
            extra_cycles+=2;
        }
        else if((signed long) ((address << 2) - branch_offset) < 0)
            extra_cycles+=2;

        break;
    default:break;


    }
}

char flagf(char cflag, bool is_set)
{
    char flag = cflag;
    switch(flag)
    {
    case 'c':
       if(is_set)
           flag = 'C';
       break;
    case 'C':
       if(!is_set)
           flag = 'c';
       break;
    case 'z':
       if(is_set)
           flag = 'Z';
       break;
    case 'Z':
       if(!is_set)
           flag = 'z';
       break;
    case 'n':
       if(is_set)
           flag = 'N';
       break;
    case 'N':
       if(!is_set)
           flag = 'n';
       break;
    case 'i':
       if(is_set)
           flag = 'I';
       break;
    case 'I':
       if(!is_set)
           flag = 'i';
       break;
    case 'b':
       if(is_set)
           flag = 'B';
       break;
    case 'B':
       if(!is_set)
           flag = 'b';
       break;
    case 'v':
       if(is_set)
           flag = 'V';
       break;
    case 'V':
       if(!is_set)
           flag = 'v';
       break;
    case 'u':
       if(!is_set)
           flag = 'U';
       break;
    case 'U':
       if(!is_set)
           flag = 'u';
       break;
    case 'd':
       if(is_set)
           flag = 'D';
       break;
    case 'D':
       if(!is_set)
           flag = 'd';
       break;

    default: break;
    }
    return flag;






}
void M6502::pretrace()
{
    pre_trace_snapshot = this->GetCPUState();
}

void M6502::trace_current_instruction(int inc)
{
    CPU_SNAPSHOT snap_current = pre_trace_snapshot;//this->GetCPUState();
    CPU_SNAPSHOT snap = pre_trace_snapshot;


    char hexstr[12];
     memset(hexstr, 0, 12);
    switch(inc)
    {
    case 3:
        sprintf(hexstr, "%02X %02X %02X", memory->ReadMemory(snap_current.pc), memory->ReadMemory(snap_current.pc + 1), memory->ReadMemory(snap_current.pc+2));
        break;
    case 2:
        sprintf(hexstr, "%02X %02X   ", memory->ReadMemory(snap_current.pc), memory->ReadMemory(snap_current.pc + 1));
        break;
    case 1:
    default:
        sprintf(hexstr, "%02X      ", memory->ReadMemory(snap_current.pc));
        break;


    }

#ifdef ENABLE_TRACING
    fprintf(tr, "$%04X:%s A:%02X X:%02X Y:%02X S:%02X P:%c%c%c%c%c%c%c%c\n", snap.pc, hexstr,snap.a,snap.x,snap.y,
            snap.SP,flagf('n', snap.fl.members.N),flagf('v', snap.fl.members.V),flagf('u', snap.fl.members.unused),flagf('b', snap.fl.members.B),flagf('d', snap.fl.members.D),flagf('i', snap.fl.members.I),flagf('z', snap.fl.members.Z) ,flagf('c', snap.fl.members.C));
#endif


    //this->dis->trace_current_instr_(wa);
}


void M6502::PrintAllRegs()
{
    printf("A: 0x%02X Y: 0x%02X X: 0x%02X S: 0x%02X SR: 0x%02X", A, Y, X, SP, flags.SR);
}
