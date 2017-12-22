#ifndef PPU3_H
#define PPU3_H

#include "famicom.h"
#include "nescart.h"
//#include "SDL/SDL.h"
#include "ppu_base.h"

typedef struct
{
    byte x;
    byte attributes;
    byte index;
    byte draw_x;
    byte tile_no;

    byte tile1, tile2;


}OAM_struct;

typedef struct
{
    byte nametable_entry;
    word chr_addr;
    byte attrib;
    byte tile_data[2];


}_pixel_prefetch;

typedef byte (*ppu3_readfunc )(word );
typedef void (*ppu3_writefunc)(word, byte);


class ppu3 : public PPU_base
{
public:
    ppu3();
    ~ppu3();

    void Reset();

    byte ReadMemory(word adress);
    void WriteMemory(word adress, byte value);



    void NMI_Triggered(){} //raised from the CPU
    void RTI_Triggered(){}//raised from the CPU


  //  void BlitSDL(SDL_Surface *src);


    void SetDrawSurface(unsigned long *sf);
    void ReleaseDrawSurface();


    bool NMI_in_VBlank(); //indicted wheter NMI occurs when vertical blank starts
    bool inVBlank(); //is the PPU in VBlank?


  //   bool draw; //now exists in superclass.

//     bool NMI_occured; //in superclass


     void SetCart(NESCart *m);

     void Reset2003() { reg_2003 = 0; }
     byte Read2003() { return reg_2003; }

     byte *GetNTPtr() { return this->nametables; }
     byte GetPal(byte num) { return palette[num];}
     byte DEBUG_EXPOSE_REGISTER(word reg);

     void Step();
     void RunCycles(int cycles);
private:
    byte reg_2000; //PPU control register
    byte reg_2001; //PPU mask register
    byte reg_2002; //PPU status register
    byte reg_2003; // OAM adress



    word current_scanline;


    NESCart *cart;
 //   byte *CHR; //0x2000 bytes of nametables and other juicy stuff ^_^

    byte *nametables; //PPU RAM
    byte *palette;
    byte OAM[0x100];
    OAM_struct tOAM[0x40]; //internal (pre-fetched) OAM
    _pixel_prefetch ppu_pixels[3];





    bool latch;
    byte fine_x;

    byte ppu_bus;
    word ppu_addr;
    word ppu_temp;


    bool sprite_zero;


    byte *draw_surface; //pointer to the surface to draw on is dependant on the rendering backend
                                 //the ppu just expects an unsigned char buffer of 0xf000 bytes

    byte *NES_PALETTE;




    void spr0check();

    long ppu_cycles;

    word pixel;

    void fill_tmp_sprbuf();
    byte sprites_in_range;
    bool even_odd_frame;

    inline void pixel_copy(byte to, byte from);
    byte scanline_pixels[269];

    long frames_drawn;

    byte nt_access;

    inline byte ReadInternal(word addr);
    inline void WriteInternal(word addr, byte data);
    inline void SetLowerPtrs(byte num, byte *data);

    byte *readptr[16];
    byte *writeptr[16];

    ppu3_readfunc readfunc[16];
    ppu3_writefunc writefunc[16];


    void SetNameTable(byte nt_num);
    void SetNameTables(byte nt1, byte nt2, byte nt3, byte nt4);










};

#endif // ppu3_H
