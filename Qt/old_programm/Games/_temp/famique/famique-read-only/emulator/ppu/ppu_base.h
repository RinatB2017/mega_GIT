#ifndef PPU_BASE_H
#define PPU_BASE_H

#include "famicom.h"
#include "nescart.h"
//#include <SDL/SDL.h>

class PPU_base
{
public:
    PPU_base();
    virtual ~PPU_base(){}

   /* byte *nametables;

    */
    bool draw;
    bool NMI_occured;
    byte *texmem;

    virtual byte *GetNTPtr() = 0;
    virtual byte GetPal(byte) = 0;
    virtual byte DEBUG_EXPOSE_REGISTER(word reg) = 0;
    virtual void Reset2003() = 0;
    virtual byte ReadMemory(word) = 0;
    virtual void WriteMemory(word, byte) = 0;
 //   virtual void BlitSDL(SDL_Surface *) = 0;
    virtual void SetCart(NESCart *m) = 0;
    virtual void RunCycles(int cycles) = 0;
    virtual bool inVBlank() = 0;
    virtual bool NMI_in_VBlank()= 0;
    virtual byte *GetDrawSurface() = 0;

    virtual byte *UpdateTexMem() = 0;

};

#endif // PPU_BASE_H
