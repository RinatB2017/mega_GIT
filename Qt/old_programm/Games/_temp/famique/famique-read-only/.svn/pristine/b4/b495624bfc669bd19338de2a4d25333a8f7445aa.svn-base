#include "tracer.h"
#include <memory.h>
#include <string.h>
#include <stdio.h>

disassembler::disassembler()
{
    disassembler(NULL);

}

disassembler::disassembler(byte *mem)
{


    //open up a FILE

    this->trace_out = fopen("6502-trace.log", "w+");

	//set up opcode tables...
	char op_codes[256][4] = {
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


	  memcpy(&opcodes ,&op_codes, 256*4);

	  unsigned char modes[256] =
{
  0, ZPIx, 0, 0, 0, ZP, ZP, 0,
  0, IMM, 0, 0, 0, ABS, ABS, 0,
  REL, ZPIy, 0, 0, 0, ZPx, ZPx, 0,
  0, ABSy, 0, 0, 0, ABSx, ABSx, 0,
  ABS, ZPIx, 0, 0, ZP, ZP, ZP, 0,
  0, IMM, 0, 0, ABS, ABS, ABS, 0,
  REL, ZPIy, 0, 0, 0, ZPx, ZPx, 0,
  0, ABSy, 0, 0, 0, ABSx, ABSx, 0,
  0, ZPIx, 0, 0, 0, ZP, ZP, 0,
  0, IMM, 0, 0, ABS, ABS, ABS, 0,
  REL, ZPIy, 0, 0, 0, ZPx, ZPx, 0,
  0, ABSy, 0, 0, 0, ABSx, ABSx, 0,
  0, ZPIx, 0, 0, 0, ZP, ZP, 0,
  /*
        m6502_adressing_modes_setup[0x65] = &M6502::zeropage;
        m6502_adressing_modes_setup[0x66] = &M6502::zeropage;
        m6502_adressing_modes_setup[0x67] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
        m6502_adressing_modes_setup[0x68] = &M6502::implied;
        m6502_adressing_modes_setup[0x69] = &M6502::immediate;
        m6502_adressing_modes_setup[0x6A] = &M6502::accumulator;
        m6502_adressing_modes_setup[0x6B] = &M6502::UNIMPLEMENTED_ADRESSING_MODE;
        m6502_adressing_modes_setup[0x6C] = &M6502::indirect;
        m6502_adressing_modes_setup[0x6D] = &M6502::absolute;
        m6502_adressing_modes_setup[0x6E] = &M6502::absolute;*/

  0, IMM, 0, 0, IND, ABS, ABS, 0,
  REL, ZPIy, 0, 0, 0, ZPx, ZPx, 0,
  0, ABSy, 0, 0, 0, ABSx, ABSx, 0,
  0, ZPIx, 0, 0, ZP, ZP, ZP, 0,
  0, 0, 0, 0, ABS, ABS, ABS, 0,
  REL, ZPIy, 0, 0, ZPx, ZPx, ZPy, 0,
  0, ABSy, 0, 0, 0, ABSx, 0, 0,
  IMM, ZPIx, IMM, 0, ZP, ZP, ZP, 0,
  0, IMM, 0, 0, ABS, ABS, ABS, 0,
  REL, ZPIy, 0, 0, ZPx, ZPx, ZPy, 0,
  0, ABSy, 0, 0, ABSx, ABSx, ABSy, 0,
  IMM, ZPIx, 0, 0, ZP, ZP, ZP, 0,
  0, IMM, 0, 0, ABS, ABS, ABS, 0,
  REL, ZPIy, 0, 0, 0, ZPx, ZPx, 0,
  0, ABSy, 0, 0, 0, ABSx, ABSx, 0,
  IMM, ZPIx, 0, 0, ZP, ZP, ZP, 0,
  0, IMM, 0, 0, ABS, ABS, ABS, 0,
  REL, ZPIy, 0, 0, 0, ZPx, ZPx, 0,
  0, ABSy, 0, 0, 0, ABSx, ABSx, 0};

  memcpy(&addr_modes, &modes, 0xff);

  memory = mem;

  this->disassembly = new char[256];
  allocated_memory = 256;
  output_offset = 0;


}

disassembler::~disassembler()
{

        delete [] disassembly;
        fclose(this->trace_out);
}

char *disassembler::DisassembleBlock(word start, byte num_instructions)
{

    //WIP
    char *ret =(char *) malloc(0x1000);
    long str_size = 0x1000;
    long dasm_size =0;

    dasm_size = disassemble(start, true);
    memcpy(ret, disassembly, dasm_size);

    for(int i=1;i<num_instructions;i++)
    {

        memcpy(ret+dasm_size, disassembly+dasm_size, dasm_size);

        if(dasm_size > (str_size - 100))
            ret = (char *)realloc(ret, str_size+0x1000);


    }

return ret;

}

int disassembler::disassemble(MemoryManager *mem, unsigned short offset)
{
    unsigned long PC = offset;
    long str_offset = 0;


    while(PC<0x10001)
    {
        if(str_offset > (allocated_memory-100))
        {
            disassembly = (char *)realloc(disassembly, allocated_memory+0x100);
            allocated_memory += 0x100;
        }

    byte opcode = mem->ReadMemory(PC);
    //memset(tmp_str, 0, 256);

    str_offset +=   sprintf(disassembly+str_offset, "%04X: ($%02X)      %s   ", PC, opcode, opcodes[opcode]);
    //str_offset += strlen(tmp_str);

    switch(addr_modes[opcode])
    {


    case ZP:
          str_offset +=  sprintf(disassembly+str_offset, "$%02X\n", mem->ReadMemory(PC+1));
            PC+=2;
            break;
    case ZPx:
          str_offset +=  sprintf(disassembly+str_offset,"$%02X, X\n",  mem->ReadMemory(PC+1));
            PC+=2;
            break;
    case REL:

            if(mem->ReadMemory(PC+1) & 0x80)
                str_offset += sprintf(disassembly+str_offset, "-%02X [$%04X]\n", (byte)(0xFF-(mem->ReadMemory(PC+1))),(word)(PC+2+(mem->ReadMemory(PC+1)-0x100)));
            else
                str_offset += sprintf(disassembly+str_offset,"+%02X [$%04X] \n", mem->ReadMemory(PC+1), (word)((PC+2) + mem->ReadMemory(PC+1)));


            PC+=2;
                    break;
    case ABSy:
         str_offset +=   sprintf(disassembly+str_offset, "$%02X%02X, Y\n", mem->ReadMemory(PC+2), mem->ReadMemory(PC+1));
            PC+=3;
            break;
    case IMM:
        str_offset +=    sprintf(disassembly+str_offset,"#%02X \n", mem->ReadMemory(PC+1));
            PC+=2;
            break;
    case ZPIy:
        str_offset +=    sprintf(disassembly+str_offset, "$%02X, Y\n", mem->ReadMemory(PC+1));
            PC+=2;
            break;

    case ZPIx:
        str_offset +=   sprintf(disassembly+str_offset, "$%02X, X\n", mem->ReadMemory(PC+1));
        PC+=2;
        break;

    case ABSx:
        str_offset +=    sprintf(disassembly+str_offset, "$%02X%02X, X\n", mem->ReadMemory(PC+2), mem->ReadMemory(PC+1));
            PC+=3;
            break;
    case ABS:
        str_offset +=    sprintf(disassembly+str_offset, "$%02X%02X\n", mem->ReadMemory(PC+2), mem->ReadMemory(PC+1));
            PC+=3;
            break;

    case IND:
            //calculate indirect adressing
            /*

                        byte hx1, hx2;
                        hx1 = memory->ReadMemory(PC+1);
                        hx2 = memory->ReadMemory(PC+2);

                        if(!check_jmp_bug_6502)
                        adress = MAKE_WORD(
                                memory->ReadMemory(MAKE_WORD(hx1, hx2)),

                                memory->ReadMemory(MAKE_WORD(hx1, hx2)+1)
                                );
                        else //we caught the jump bug ^_^
                                adress = MAKE_WORD(memory->ReadMemory(MAKE_WORD(hx1, hx2)),
                                memory->ReadMemory(MAKE_WORD(0x00, hx2)));
*/

            byte t1, t2;
            t1 = mem->ReadMemory(PC+1);
            t2 = mem->ReadMemory(PC+2);

            str_offset += sprintf(disassembly+str_offset, "($%04X) ;=$%04X !!!does not disassemble properly for indirected jumps", MAKE_WORD(t1,t2),
                                  MAKE_WORD(mem->ReadMemory(MAKE_WORD(t1,t2)), mem->ReadMemory(MAKE_WORD(t1,t2))+1));

    default:
       str_offset +=     sprintf(disassembly+str_offset, "\n");
            PC++;
            break;

    }





//	memcpy(disassembly, tmp_str, strlen(tmp_str));
//	output_offset += strlen(tmp_str);
    //printf("%s\n", tmp_str);

    /*if(PC==0x8FFF)
            PC=PC;
    */


}




return str_offset;




}

int disassembler::disassemble(unsigned short offset, bool one_line)
{

	byte *mem = memory;

	
	long PC = offset;

        long str_offset = 0;



        while(PC<0x10001)
	{
            if(str_offset> (allocated_memory-100))
            {
                disassembly = (char *)realloc(disassembly, allocated_memory+0x100);
                allocated_memory += 0x100;
            }

	byte opcode = mem[PC];
        //memset(tmp_str, 0, 256);

        str_offset +=   sprintf(disassembly+str_offset, "%04X:       %s   ", PC, opcodes[opcode]);
        //str_offset += strlen(tmp_str);

        switch(addr_modes[opcode])
	{


	case ZP:
              str_offset +=  sprintf(disassembly+str_offset, "$%02X\n", mem[PC+1]);
		PC+=2;
		break;
	case ZPx:
              str_offset +=  sprintf(disassembly+str_offset,"$%02X, X\n", mem[PC+1]);
		PC+=2;
		break;
	case REL:
	
		if(mem[PC+1] & 0x80)
                    str_offset +=        sprintf(disassembly+str_offset, "-%02X [$%04X]\n", (byte)(0xFF-(mem[PC+1])), (PC+2)+(mem[PC+1]-0x100));
		else
              str_offset +=          sprintf(disassembly+str_offset,"+%02X [$%04X] \n", mem[PC+1], (PC+2) + mem[PC+1]);
	

		PC+=2;
			break;
	case ABSy:
             str_offset +=   sprintf(disassembly+str_offset, "$%02X%02X, Y\n", mem[PC+2], mem[PC+1]);
		PC+=3;
		break;
	case IMM:
            str_offset +=    sprintf(disassembly+str_offset,"#%02X \n", mem[PC+1]);
		PC+=2;
		break;
	case ZPIy:
            str_offset +=    sprintf(disassembly+str_offset, "$%02X, Y\n", mem[PC+1]);
		PC+=2;
		break;

        case ZPIx:
            str_offset +=   sprintf(disassembly+str_offset, "$%02X, X\n", mem[PC+1]);
            PC+=2;
            break;
	
	case ABSx:
            str_offset +=    sprintf(disassembly+str_offset, "$%02X%02X, X\n", mem[PC+2], mem[PC+1]);
		PC+=3;
		break;
	case ABS:
            str_offset +=    sprintf(disassembly+str_offset, "$%02X%02X\n", mem[PC+2], mem[PC+1]);
		PC+=3;
		break;

	default:
           str_offset +=     sprintf(disassembly+str_offset, "\n");
                PC++;
		break;

	}





//	memcpy(disassembly, tmp_str, strlen(tmp_str));
//	output_offset += strlen(tmp_str);
	//printf("%s\n", tmp_str);

	/*if(PC==0x8FFF)
		PC=PC;
        */

        if(one_line)
            break;

    }




   return str_offset;



}



/*This function is meant to be called after the CPU passes through the mnemonic execution and memory access*/
void disassembler::trace_current_instr_(char *s)
{
    fprintf(trace_out, "%s", s);


}
