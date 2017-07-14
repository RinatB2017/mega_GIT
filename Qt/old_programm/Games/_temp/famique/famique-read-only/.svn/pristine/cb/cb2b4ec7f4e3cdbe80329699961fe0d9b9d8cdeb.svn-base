#ifndef PPU2_H
#define PPU2_H

#include "famicom.h"
#include "nescart.h"
//#include "SDL/SDL.h"
#include "ppu_base.h"

class ppu2 : public PPU_base
{
public:
    ppu2();
    ~ppu2();


    void setCHR(byte *PRG);

    byte ReadMemory(word adress);
    void WriteMemory(word adress, byte value);


    void do_scanline();

    void NMI_Triggered(){} //raised from the CPU
    void RTI_Triggered(){}//raised from the CPU


  //  void BlitSDL(SDL_Surface *src);


    void SetDrawSurface(unsigned long *sf);
    void ReleaseDrawSurface();


    bool NMI_in_VBlank(); //indicted wheter NMI occurs when vertical blank starts
    bool inVBlank(); //is the PPU in VBlank?


    // bool draw; //now exists in superclass

   //  bool NMI_occured; // in superclass


     void SetCart(NESCart *m) { cart = m; }


     void Reset2003() { reg_2003 = 0; }
     byte Read2003() { return reg_2003; }

     void UGLY_HAX_PPU_CATCH_UP(word num_cpu_cycles);
     byte *GetNTPtr() { return this->nametables; } //in superclass
     byte GetPal(byte num) { return palette[num];}//in superclass
     byte DEBUG_EXPOSE_REGISTER(word reg);
     byte *GetDrawSurface() { return draw_surface;}

     byte *UpdateTexMem();
private:
    byte reg_2000; //PPU control register
    byte reg_2001; //PPU mask register
    byte reg_2002; //PPU status register
    byte reg_2003; // OAM adress

    void RunCycles(int cycles);
    int cycles_internal;



    void WriteInternal(word adress, byte value);
    byte ReadInternal(word adress);

    void scanline_sprites(int nScanline, bool front);
    void scanline_background(int scanline);

    word current_scanline;


    NESCart *cart;
 //   byte *CHR; //0x2000 bytes of nametables and other juicy stuff ^_^

    byte *nametables; //PPU RAM
    byte *palette;
    byte OAM[0x100];





    bool latch;
    byte scroll_x, scroll_y;

    byte ppu_bus;
    word ppu_address;
    word ppu_address_prev;

    byte temp_register;

    bool sprite_zero;
    int sprite_overflow;


    byte *draw_surface; //pointer to the surface to draw on is dependant on the rendering backend
                                 //the ppu just expects an unsigned long buffer of 0xf000 bytes

    byte *NES_PALETTE;




    void spr0check();

    long ppu_cycles;






};

#endif // PPU2_H
