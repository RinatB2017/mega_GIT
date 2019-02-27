#pragma once
#ifndef PALETTE_H__
#define PALETTE_H__

#ifndef byte
#define byte unsigned char
#endif
#include "famicom.h"

#include <SDL.h>

struct _nesRGBStruct
{
    byte r,g,b;

};
_nesRGBStruct *ConvertToRGBStruct(byte *pal);

//byte *NES_COLORS_TO_RGB;
byte* SetupPalette();
unsigned long *ConvertToPalette32(byte *pal8);

void SetSDLPalette(SDL_Surface *);

#endif
