#pragma once
#ifndef FAMICOM_H
#define FAMICOM_H


#ifndef byte
#define byte unsigned char
#endif
#ifndef word
#define word unsigned short
#endif
#ifndef dword
#define dword unsigned long
#endif


#define GET_BIT(x, bitnum) (x>>bitnum)&0x1

#define SET_BIT(x, bitnum) (x |= 1<<bitnum)
#define CLR_BIT(x, bitnum) (x &= ~(1 << bitnum) & 0xFF)

#define FILE_OPEN_ERROR 0xF00F
#define FILE_READ_ERROR 0xF00E
#define FILE_WRONG_FILETYPE 0xF00D
#define FILE_SRAM_READONLY 0xF00C

#define LOW_ENDIAN

#define MAKE_WORD(byte1, byte2) (byte2<<8|byte1)

#define ROM_OFFSET 0xC000

#define SR(x) (x.C & 0x1 | x.Z <<1 & 0x1 | x.I <<2 & 0x1 | x.D << 3 & 0x1 | x.B << 4 & 0x1 | x.unused << 5 & 0x1 | x.V << 6 & 0x1 | x.S << 7 & 0x1)

#define LSB(x) ( x & 0xFF)
#define MSB(x) ( x >> 8)

#define NTSC_CYCLES_PER_HBLANK 113

#define MEM_4k  0x1000
#define MEM_8k  0x2000
#define MEM_16k 0x4000


#include <stdlib.h>



typedef struct fch
{
        byte NES_IDENT[4]; //should be NES^Z

	byte num_16kb_rom_banks;
	byte num_8kb_vrom_banks;
	byte info1; /*bit 0     1 for vertical mirroring, 0 for horizontal mirroring.
			      bit 1     1 for battery-backed RAM at $6000-$7FFF.
			      bit 2     1 for a 512-byte trainer at $7000-$71FF.
			      bit 3     1 for a four-screen VRAM layout. 
			      bit 4-7   Four lower bits of ROM Mapper Type.                 */
	byte info2;
         byte num_8kb_ram_banks; //if zero, assume 1x8kb
	byte video_mode; //first bit 1 = pal, 0 = ntsc. bit 1-7 , must be NULL
	byte reserved[6];//must be NULL
}famicom_header;

class iNesLoader
{
public:
	iNesLoader(){ 
	
	};
	~iNesLoader(){
		free(data);
		free(CPU_MEMORY_MAP);
		free(PPU_MEMORY_MAP);

	};

	int Open(const char *filename);
	int Close();
        word GetInitExec() { return MAKE_WORD(CPU_MEMORY_MAP[0xfffc], CPU_MEMORY_MAP[0xfffd]); }
	
	int num_16kb_rombanks;
	int num_8kb_rombanks;
	famicom_header head;
	bool trainer_present;
	byte mapper;
	long filesize;
	void Load16kbROM();
	void Load8kbVROM();

	byte *data;
	byte *CPU_MEMORY_MAP;
	byte *PPU_MEMORY_MAP;

	bool horizontal_mirroring;
	bool vertical_mirroring;

};


#endif
