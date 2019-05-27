

#ifndef __TRACER_H
#define __TRACER_H

#include "famicom.h"
#include "../memory.h"
//#include "cpu2.h"
#include <stdio.h>

#define ZP 1
#define ZPx 2
#define ZPy 3
#define ZPIx 4
#define ZPIy 5
#define ABS 6
#define ABSx 7
#define ABSy 8
#define IND 9
#define REL 10
#define IMM 11



class disassembler
{
public:

	disassembler(byte *mem);
        disassembler();
	~disassembler();
int 	disassemble(word offset, bool one_line = false);

char *GetDisassembly() { return disassembly;}
    char *DisassembleBlock(word start, byte num_instructions);

    int disassemble(MemoryManager *mem, word offset);

    void trace_current_instr_(char *s);
    char *GetOpMnem(byte op) {return  (char *)opcodes[op]; }
private:

	byte *memory;
	char opcodes[256][4];
	char addr_modes[256];

	long output_offset;
	char *disassembly;
        long allocated_memory;


        FILE *trace_out;

};




#endif
