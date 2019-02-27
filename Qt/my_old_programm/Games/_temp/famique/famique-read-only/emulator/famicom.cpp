#define _CRT_SECURE_NO_WARNINGS
#include "famicom.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>


int iNesLoader::Open(const char *filename)
{

		
	memset(&head, 0, sizeof(famicom_header));

	horizontal_mirroring = vertical_mirroring = 0;
		

	FILE *fp = fopen(filename, "rb");
	if(!fp)
		return FILE_OPEN_ERROR;

	fseek(fp, 0, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);

	if( fread(&head, sizeof(famicom_header), 1, fp) != 1)
		return FILE_READ_ERROR;

	
	if(head.NES_IDENT[0] != 'N' || head.NES_IDENT[1] != 'E' || head.NES_IDENT[2] != 'S')
		return FILE_WRONG_FILETYPE;
	
	mapper = (head.info1 & 0xf0) >> 4;
	mapper = mapper | (head.info2 & 0xf0);
	
	
//	free(head);
	
	data = (byte *)malloc(filesize - sizeof(famicom_header));
	CPU_MEMORY_MAP = (byte *)malloc(0x10000);
	PPU_MEMORY_MAP = (byte *)malloc(0x4000);
	memset(CPU_MEMORY_MAP, 0, 0x10000);
	memset(PPU_MEMORY_MAP, 0, 0x4000);
	
	
	if(fread(data, filesize - sizeof(famicom_header), 1, fp) != 1)
		return FILE_READ_ERROR;

	if(!GET_BIT(head.info1, 0))
		horizontal_mirroring = 1;
	else
		vertical_mirroring = 1;
	
	
	fclose(fp);
	return 0;
}

void iNesLoader::Load16kbROM()
{
        memcpy(CPU_MEMORY_MAP+0x8000, data, 0x4000);
        if(head.num_16kb_rom_banks == 1)
        {
                memcpy(CPU_MEMORY_MAP+0xC000, data, 0x4000);
        }
        else if (head.num_16kb_rom_banks == 2)
        {
		memcpy(CPU_MEMORY_MAP+0xC000, data+0x4000, 0x4000);
	}
	
}

void iNesLoader::Load8kbVROM()
{
	memcpy(PPU_MEMORY_MAP, data + (head.num_16kb_rom_banks * 0x4000), 0x2000);	
}
