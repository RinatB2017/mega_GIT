//#include "SDL/SDL.h"
#include "palette.h"
#include <string.h>
_nesRGBStruct *ConvertToRGBStruct(byte *pal)
{
    return (_nesRGBStruct*)pal;
}

byte * SetupPalette()
{


    byte COLORS[] =
    {
    117,117,117
    ,39,27,143
    ,0,0,171
    ,71,0,159
    ,143,0,119
    ,171,0,19
    ,167,0,0
    ,127,11,0
    ,67,47,0
    ,0,71,0
    ,0,81,0
    ,0,63,23
    ,27,63,95
    ,0,0,0
    ,0,0,0
    ,0,0,0
    ,188,188,188
    ,0,115,239
    ,35,59,239
    ,131,0,243
    ,191,0,191
    ,231,0,91
    ,219,43,0
    ,203,79,15
    ,139,115,0
    ,0,151,0
    ,0,171,0
    ,0,147,59
    ,0,131,139
    ,0,0,0
    ,0,0,0
    ,0,0,0
    ,255,255,255
    ,63,191,255
    ,95,151,255
    ,167,139,253
    ,247,123,255
    ,255,119,183
    ,255,119,99
    ,255,155,59
    ,243,191,63
    ,131,211,19
    ,79,223,75
    ,88,248,152
    ,0,235,219
    ,0,0,0
    ,0,0,0
    ,0,0,0
    ,255,255,255
    ,171,231,255
    ,199,215,255
    ,215,203,255
    ,255,199,255
    ,255,199,219
    ,255,191,179
    ,255,219,171
    ,255,231,163
    ,227,255,163
    ,171,243,191
    ,179,255,207
    ,159,255,243
    ,0,0,0
    ,0,0,0
    ,0,0,0};

    byte *cpcolors = new byte[sizeof(COLORS)];

    memcpy(cpcolors, COLORS, sizeof(COLORS));


    return cpcolors;
}
unsigned long *ConvertToPalette32(byte *pal8)
{

    return (unsigned long *)pal8;

}

#ifdef NOSDL
void SetSDLPalette(SDL_Surface *surf)
{
	SDL_Color pal[256];
        byte *NES_COLORS_TO_RGB = SetupPalette();
	memset(&pal, 0, 256);
	int index;
	int nIndex = 0;
	byte tempcol=0;
	
	byte index_off;
	for(index=0;index<0x64;index++)
	{
		index_off = index * 3;
		pal[index].r = NES_COLORS_TO_RGB[index_off];
		pal[index].g = NES_COLORS_TO_RGB[index_off+1];
		pal[index].b = NES_COLORS_TO_RGB[index_off+2];
		
		/*tempcol = nes_palette[nIndex] >> 16 & 0xFF;
		pal[index].r = tempcol;
		tempcol = nes_palette[nIndex] >> 8 & 0xFF;
		pal[index].g = tempcol;
		tempcol = nes_palette[nIndex] & 0xFF;
		pal[index].b = tempcol;
		*/
	
		
	}

	SDL_SetColors(surf, pal, 0, 256);

	


}
#endif

