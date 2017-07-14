
#include <memory.h>
#include "ppu3.h"
#include "ppu/palette.h"
#include <string.h>
/*

 skriv om read/write & scanline... det behvs...*/

#define SPRITES_ENABLED ((reg_2001 & 0x10) == 0x10)
#define BG_ENABLED ((reg_2001 & 0x8) == 0x8)
#define BG_SPR_ENABLED ((reg_2001 & 0x18) == 0x18)

#define NT1 0
#define NT2 0x400
#define NT3 0x800
#define NT4 0xC00;



ppu3::ppu3()
{


    ppu_bus = ppu_addr = 0;
    this->current_scanline = 0;

    this->nametables = new byte [0x1000];
    memset(nametables, 0, 0x1000);


    this->draw_surface = new byte [0xf00F];

    this->palette = (byte *)new byte [0x20];
    memset(palette, 0, 0x20);

    memset(this->OAM, 0x00, 0xFF);
    memset(this->tOAM, 0x00, sizeof(OAM_struct)*64);

    latch = false;

    draw = false;

    reg_2000 = 0;
    this->reg_2001 = 0;
    this->reg_2002 = 0;
    this->reg_2003 = 0;

    fine_x  = ppu_temp = 0;


     this->NES_PALETTE = SetupPalette();
     ppu_cycles = 0;

     pixel = 0;
     nt_access =0;
     current_scanline = 0;
     this->even_odd_frame = 0;

     this->SetNameTable(0);
    this->SetNameTable(1);
    this->SetNameTable(2);
    this->SetNameTable(3);


 }



ppu3::~ppu3()
{

    delete [] draw_surface;
    delete [] nametables;
    //cart  = 0;
    delete [] NES_PALETTE;
    delete [] palette;
    //

}

void ppu3::SetCart(NESCart *m)
{
    cart = m;
    this->SetLowerPtrs(0, this->cart->mROMMapper->bCHRBanks[0]);

    this->SetLowerPtrs(1, this->cart->mROMMapper->bCHRBanks[0]);

    this->SetLowerPtrs(2, this->cart->mROMMapper->bCHRBanks[0]);
         this->SetLowerPtrs(3, this->cart->mROMMapper->bCHRBanks[0]);
         this->SetLowerPtrs(4, this->cart->mROMMapper->bCHRBanks[0]);
         this->SetLowerPtrs(5, this->cart->mROMMapper->bCHRBanks[0]);
         this->SetLowerPtrs(6, this->cart->mROMMapper->bCHRBanks[0]);
         this->SetLowerPtrs(7, this->cart->mROMMapper->bCHRBanks[0]);
}

void ppu3::Reset()
{
    for(int i=0;i<16;i++)
    {
         this->readptr[i]  = this->writeptr[i] =0;

    }

}


bool ppu3::inVBlank()
{

    return (bool)GET_BIT(reg_2002, 7)==1 ? true:false;


}

bool ppu3::NMI_in_VBlank()
{
    return (bool)GET_BIT(reg_2000, 7)==1 ? true:false;
}



byte ppu3::ReadMemory(word adress)
{

    byte ret_byte = ppu_bus;
    word temp_address;
    switch(adress)
    {


    case 0x2002:
        ret_byte = reg_2002 | (ppu_bus & 0x1f);
        reg_2002 &= 0x7F; //clear NMI bit
        latch = 0;




    break;

    case 0x2004:

       /* if( ((reg_2001 & 0x8) == 0x8) && ((reg_2001 & 0x10)==0x10) && this->current_scanline<240 )
        {
         ret_byte = OAM[reg_2003++];
        }
        else*/
           ret_byte = OAM[reg_2003];

    break;

    case 0x2007:




    if(ppu_addr < 0x3F00)
    {


        ret_byte = ppu_bus;

    if(ppu_addr < 0x2000)
    {
        ppu_bus =  ReadInternal(ppu_addr);//cart->mROMMapper->CHRPtr[ppu_address];
    }




    else if(ppu_addr >=0x2000 && ppu_addr < 0x3f00)
    {

        /*if(ppu_addr>=0x3000)
            temp_address = (ppu_addr-0x1000 - 0x2000) & 0xFFF;
        else temp_address = (ppu_addr - 0x2000) & 0xFFF;
        //else
          //  temp_address = ppu_address - 0x2000;

          //  temp_address = (ppu_address - 0x2000 ) &0x7FF;


        //either a read from the nametables or one of its mirrors
        */ppu_bus = ReadInternal(temp_address);

    }

}






   if(ppu_addr>=0x3f00 && ppu_addr <0x3fff)
   {
            ret_byte = palette[(ppu_addr & 0x1f)];
         //   ret_byte = ppu_bus;

            /* this is some crazy ass shit right here;
               refer to http://nesdev.parodius.com/bbs/viewtopic.php?t=3554&postdays=0&postorder=asc&start=15&sid=f4b98bcfbcba352399f102431cebef70
               blargg and disch explains it fairly well :)
            */
            ppu_bus = nametables[(ppu_addr & 0x7ff)];
   }





        ppu_addr += (GET_BIT(reg_2000, 2)? 32:1);




        ppu_addr &= 0x3fff;




       break;

    default:
    printf("PPU read out of bounds [0x%04X", adress);
    break;

    }


    return ret_byte;

}



void ppu3::WriteMemory(word adress, byte val)
{

    switch(adress)
    {

    case 0x2000:
        reg_2000 = val;
        this->ppu_temp &= ~(3 << 10);
        ppu_temp |= (val & 3) << 10;
    break;

    case 0x2001:
        this->reg_2001 = val;

    case 0x2003:
        this->reg_2003 = val;
    break;

    case 0x2004:
        OAM[reg_2003] = val;
        reg_2003 = (reg_2003 +1) & 0xFF;
    break;

    case 0x2005:

    {
        if(!this->latch)
        {
            ppu_temp &= ~0x1F;
            ppu_temp |= (val >> 3);
            fine_x = (val &7);
            latch = true;

        }
        else
        {
            ppu_temp &= ~((0x1F << 5) | (7<<12));
            ppu_temp |= (val & ~7) << 2;
            ppu_temp |= (val & 7) << 12;
            latch = false;

        }

    }
       break;

     case 0x2006:
       {
           if(!latch)
           {
               ppu_temp &= 0xFF;
               ppu_temp |= (val & 0x3F) << 8;
               latch = true;
           }
           else
           {
               ppu_temp &= ~0xFF;
               ppu_temp |= val;
               this->ppu_addr = ppu_temp;
               latch = false;
           }

       }




    break;


    case 0x2007:

    //ppu_addr &= 0x3FFF;
    if(ppu_addr <= 0x2000) /*
                               CHR ROM (R/O)
                               CHR RAM (R/W)
 */  cart->mROMMapper->WriteCHR(ppu_addr, val); //        cart->mROMMapper->CHRPtr[ppu_address] = val;

  else if(ppu_addr < 0x3f00)
    {
       /* if(ppu_addr >= 0x3000)
            ppu_addr &= 0x2fff;
*/

      WriteInternal(ppu_addr &0x2FFF, val);
      }

    else if(ppu_addr>=0x3f00 && ppu_addr<=0x3fff)
    {




       //palette + mirrors
/*
                    nametables[ppu_address &0x1f1f]=val;
                    if(!((ppu_address &0x1f1f)&0x7))
                            nametables[((ppu_address&0x1f1f)^0x10)] = val;
*/
        palette[ (ppu_addr-0x3f00)&0x1f] = val;
   /*     if(!((ppu_address & 0x1f1f) & 0x7))
            palette[((ppu_address-0x3f00)&0x1f)^0x10] = val;
*/
        if(!((ppu_addr & 0x1f) & 0x7))
            palette[((ppu_addr & 0x1f)^0x10)] = val;



    }



    ppu_addr += GET_BIT(reg_2000, 2)? 32:1; //increment by 32 if bit is set, 1 if NULL





    break;


default:break;


    }





}



void ppu3::spr0check()
{

    if(this->current_scanline >= (OAM[0] +(GET_BIT(reg_2000, 5)==1?16:8)-1))
        sprite_zero = true;
    else
        sprite_zero = false;


}
//#define NEW_TEST_PPU

/*void ppu3::do_scanline()
{
    //at the start (end) of every scanline, set new HSCROLL


    if(current_scanline==0)
    {
        //end of VBLANK
        draw=false;
#ifndef NEW_TEST_PPU
        CLR_BIT(reg_2002, 7);
        //       sprite_zero = 0;

#endif



            //   reg_2002 = 0;
    }

    else  if(current_scanline < 241)
    {
        //this is where all the frame action takes place...

        sprite_overflow = 0;
        sprite_zero = 0;
        int current_scanline_mult = (current_scanline - 1) * 256;



            for(int i=0;i<256;i++)
            {
            //clear current scanline to background color
            this->draw_surface[current_scanline_mult+i] =palette[0];
            //clear sprite zero buffer
            }





        if(!(reg_2001 & 0x2))//background clipping on?
        {
                for(int i=0;i<8;i++)
                    this->draw_surface[((current_scanline-1)*256)+i] = this->palette[0];

        }



        if((reg_2001 & 0x10) == 0x10)
         this->scanline_sprites(this->current_scanline-1, false);
        if((reg_2001 & 0x8) == 0x8 )
         this->scanline_background(this->current_scanline-1);
        if((reg_2001 & 0x10) == 0x10)
         this->scanline_sprites(this->current_scanline-1, true);


    //

       if ((reg_2001&0x10) == 0x10 && (reg_2001 & 0x8))
            this->spr0check();

       // if( (reg_2001 & 0x8) == 0x8 && (reg_2001 & 0x10))
         //   this->spr0check();


         if(sprite_zero)
             SET_BIT(reg_2002, 6);
         else
             CLR_BIT(reg_2002, 6);



         if(sprite_overflow > 9)
             SET_BIT(reg_2002, 5);
         else
             CLR_BIT(reg_2002, 5);








    }

    //idle scanline at 241
   else if(current_scanline == 242)
    {
       //vblank starts here
#ifndef NEW_TEST_PPU
       SET_BIT(reg_2002,7);

      CLR_BIT(reg_2002, 6); //reset sprite zero
       CLR_BIT(reg_2002, 5);
#endif

     }

   if(current_scanline == 261)
   {
        //here we our internal surface to SDL
       current_scanline = 0;


       draw = true;

       return;
   }

   current_scanline++;





}*/

/*void ppu3::new_background(int nScanline)
{




}*/

#define PPU_CLOCK_PER_HLINE 341
/*void ppu3::UGLY_HAX_PPU_CATCH_UP(word num_cpu_cycles)
{
    this->ppu_cycles += (num_cpu_cycles * 3);

if(ppu_cycles > 0 && ppu_cycles < PPU_CLOCK_PER_HLINE)
    CLR_BIT(reg_2002,7);

else if(ppu_cycles > 89001)
{
    SET_BIT(reg_2002, 7);
    CLR_BIT(reg_2002, 6);
    CLR_BIT(reg_2002, 5);
}
else if(ppu_cycles >89342)
    ppu_cycles = 0;
}*/

/*void ppu3::scanline_background(int nScanline)
{

    int scanline = nScanline;
    byte palette_high = 0;
    //compute name table offset...
//	word name_table_offset = GET_BIT(reg_2000, 0) << 1;
//	name_table_offset |= GET_BIT(reg_2000,1);
    word name_table_offset = 0;
    word name_table_base = 0;

    int draw_offset = 0;
    //update the name table offset

    switch( (reg_2000 & 0x3))
    {
    case 0:
            name_table_offset = 0x000;

            break;
    case 1:
            name_table_offset = 0x400;
            break;
    case 2:
            name_table_offset = 0x800;
            break;
    case 3:
            name_table_offset = 0xC00;
            break;
    default:break;
    }



    //get offset in the name table data...










    byte col = 0;
    byte tile1 = 0; // the tile data
    byte tile2 = 0;

    byte tile_x_limit = 0;
    byte tile_x_start = 0;


      for(int scroll_side = 0; scroll_side < 2; scroll_side++)
    {

          name_table_base = name_table_offset; //<--- check thi out!

          int scroll_scanline = (nScanline ) + scroll_x;

          if(scroll_scanline < 0 )
            scroll_scanline = 0;



            if(scroll_side == 0)
            {
                    if(scroll_scanline >= 240)
                    {
                            switch(name_table_offset)
                            {
                            case 0:
                                    name_table_base = 0x800;// 0x800;
                            break;
                            case 0x400:
                                    name_table_base =0xC00;// 0xC00;
                            break;
                            case 0x800:
                                    name_table_base = 0x000;
                            break;
                            case 0xC00:
                                    name_table_base = 0x400;
                            break;
                            }

                            scroll_scanline = scroll_scanline - 240;
                    }
                    tile_x_start = scroll_y / 8;
                    tile_x_limit = 32;

            }
            else
            {
                    if(scroll_scanline >= 240)
                    {
                            switch(name_table_offset)
                            {
                            case 0x000:
                                    name_table_base = 0xc00;
                            break;
                            case 0x400:
                                    name_table_base = 0x800;
                            break;
                            case 0x800:
                                    name_table_base = 0x400;
                            break;
                            case 0xc00:
                                    name_table_base = 0x000;
                            break;
                            }
                            scroll_scanline = scroll_scanline - 240;
                    }
                    else
                    {
                            switch(name_table_offset)
                            {
                            case 0x000:
                                    name_table_base = 0x400;
                            break;
                            case 0x400:
                                    name_table_base = 0x000;
                            break;
                            case 0x800:
                                    name_table_base = 0xC00;
                            break;
                            case 0xC00:
                                    name_table_base = 0x800;
                            break;
                            }
                    }
                    tile_x_start = 0;
                    tile_x_limit = (scroll_y / 8) + 1;
            }



    if(cart->mROMMapper->bVertical)
    {
            switch(name_table_base)
            {

            case 0x800:
                    name_table_base= 0x000;
            break;
            case 0xC00:
                    name_table_base = 0x400;
            break;
            }
    }
    else if(cart->mROMMapper->bHorizontal)
    {
            switch(name_table_base)
            {

            case 0:
                name_table_base = 0x00;
                break;

            case 0x400:
                    name_table_base = 0x000;
            break;
            case 0x800:
                    name_table_base = 0x400;
            break;
            case 0xC00:
                    name_table_base = 0x400;
            break;
            }
    }
    if(cart->mROMMapper->bSingle)
       name_table_base = cart->mROMMapper->wSingleMirroringBase;


            for(int tile_x = tile_x_start; tile_x < tile_x_limit; tile_x++)

            {


                    byte tile_number = nametables[name_table_base + ((scroll_scanline/8)*32)+ tile_x];

                    word tile_offset = (GET_BIT(reg_2000, 4) == 1? 0x1000 : 0x0000) + (tile_number * 16); //bit 4 in $2000 determines which tile-table is for the BG

                    tile1 = cart->mROMMapper->ReadCHR((tile_offset + (scroll_scanline%8)));//cart->mROMMapper->CHRPtr[tile_offset +(scroll_scanline%8)];

                    tile2 = cart->mROMMapper->ReadCHR((tile_offset + (scroll_scanline%8)+8));// cart->mROMMapper->CHRPtr[tile_offset +(scroll_scanline%8)+8];


                    //compute upper two bits of the palette from the attribute table of the current name table

                    palette_high = nametables[name_table_base + 0x3c0 + (((scroll_scanline/8)/4) * 8) + (tile_x / 4)];

                palette_high = (byte)(palette_high >> ((4 * (((scroll_scanline / 8) % 4) / 2)) + (2 * ((tile_x % 4) / 2))));

                    palette_high = (palette_high & 0x3) << 2;



                    byte tile_pixel_start;
                    byte tile_pixel_end;

                    if(scroll_side == 0)
                    {
                            if(tile_x == tile_x_start)
                            {
                                    tile_pixel_start =  scroll_y % 8;
                                    tile_pixel_end = 8;
                            }
                            else
                            {
                                    tile_pixel_start = 0;
                                    tile_pixel_end = 8;
                            }
                    }
                    else
                    {
                            if(tile_x == tile_x_limit)
                            {
                                    tile_pixel_start = 0;
                                    tile_pixel_end = scroll_y % 8;
                            }
                            else
                            {
                                    tile_pixel_start = 0;
                                    tile_pixel_end = 8;
                            }
                    }

                    for(int tx=tile_pixel_start;tx<tile_pixel_end;tx++)

                    {


                            col = palette_high + (((tile2 & (1 << (7 - tx))) >> (7 - tx)) << 1) +
                          ((tile1 & (1 << (7 - tx))) >> (7 - tx));

                            if( (col % 4) != 0)
                            {
                                    if(scroll_side == 0)
                                    {
                                            draw_offset = scanline * 256 + (8 * tile_x) - scroll_y + tx;
                                    //	if(draw_offset < 0xf000)
                                            draw_surface[draw_offset] = palette[col];


                                   }
                                    else
                                    {
                                            if(((8*tile_x)+ (256 - scroll_y) + tx) < 256)
                                            {
                                                    draw_offset =  (scanline * 256) + (8*tile_x) + (256 - scroll_y)+ tx;
                                            //	if(draw_offset < 0xf000)
                                                    draw_surface[draw_offset] = palette[col];
                                            //	else

                                            }
                                    }
                            }





                    }




            }

    }
}

void ppu3::scanline_sprites(int nScanline, bool front)
{

    byte oam_x, oam_y, oam_attr, oam_num;


    word offset = 0;

    bool sprite_size = (reg_2000 & 0x20); //sixth bit denotes sprite size

    byte t1, t2;
    byte col_high_bits;
    byte col;

    int scanline = nScanline;

    int i = 252;
    do
    {
        oam_x = OAM[i+3];
        oam_attr = OAM[i+2];
        oam_num = OAM[i+1];
        oam_y = OAM[i]+1;


        //if(sprite_overflow>=9)
       //     return;


        //check if sprite is in range vertically

        if(oam_y <= scanline && (oam_y + (sprite_size ? 16 : 8) > scanline) &&  (oam_attr & 0x20) == (front?0x00:0x20) )
        {

            //sprite is in range

            sprite_overflow++;

            col_high_bits = (oam_attr & 0x3)<<2; //get upper bits of colour

            if(!sprite_size) // 8x8 sprites
            {

                byte tempY = scanline - oam_y;
                if((oam_attr & 0x80) == 0x80)
                    tempY = (oam_y + 7) - scanline;

                offset = (reg_2000 & 0x8) ? 0x1000 : 0x0000;
                                //set name table
                offset += (oam_num * 16); //select tile
                offset+=tempY;

            }
            else // 8x16 sprites
            {*/
                /* disch @ nesdev posted this */
/*
               byte tempY = scanline - oam_y;
               if(tempY < 16)//in range
                   if(oam_attr &0x80)//vertical flip?
                       tempY ^= 15;
               byte tile = oam_num & 0xFE;

               if(tempY&8)
                   tile++;
               tempY&=7;
*/
               /*tile banks differ from 8x16 and 8x8 sprites
                 if 8x8, tilebank is is taken from $2000
                 if 8x16, tilebank is taken from first bit of oam_num
                 */
         /*offset = (oam_num & 1) ? 0x1000: 0;

               offset += (tile * 16)+tempY;



            }



            t1 = cart->mROMMapper->ReadCHR(offset);// cart->mROMMapper->CHRPtr[offset];
            t2 = cart->mROMMapper->ReadCHR(offset+8);

            for(int sx=0;sx<8;sx++)
            {

                if((oam_attr & 0x40) == 0x40) //flip horizontally?
                     col = col_high_bits + (((t2 & (1 << (sx))) >> (sx)) << 1) +((t1 & (1 << (sx))) >> (sx));

                else
                      col = col_high_bits + (((t2 & (1 << (7 - sx))) >> (7 - sx)) << 1) +((t1 & (1 << (7 - sx))) >> (7 - sx));



                if(col%4) //is color non-transparent?
                   {
                       if( (oam_x +sx) < 256)
                       {

                          this->draw_surface[scanline * 256 + oam_x +sx] = palette[0x10 + col];


                       }

                       //if we have a non-zero pixel and we're drawing sprite zero, set the flag
                       if(i==0)
                           this->sprite_zero = 1;

                   }
                else //transparent pixel, sprite_zero doesn't fire.
                {
                if(i==0)
                        sprite_zero = 0;
                }



          }



      }
        i-=4;
    }while(i != -4);
}*/


#ifdef IGNORE_FOR_NOW
void ppu3::BlitSDL(SDL_Surface *src)
{

    Uint32 *ptr;
    int nes_pal_entry = 0;
    int yoffs=0;
    int ymul = 0;
    Uint32 *pal32 = ConvertToPalette32(NES_PALETTE);
   _nesRGBStruct *ghetto = ConvertToRGBStruct(NES_PALETTE);
    for(int y =0;y < src->h; y++)
    {
        yoffs = y * src->pitch/4;
        ymul = y * 256;
        for(int x=0;x <src->w; x++)
        {
        ptr = (Uint32 *)src->pixels + yoffs + x;
        nes_pal_entry = draw_surface[x+yoffs];
        *ptr = ghetto[nes_pal_entry].r << 16 | ghetto[nes_pal_entry].g <<8 | ghetto[nes_pal_entry].b;

        //NES_PALETTE[nes_pal_entry]<<16 +NES_PALETTE[nes_pal_entry+1] << 8 + NES_PALETTE[nes_pal_entry+2];

        }
    }
   // memcpy(bitmap->pixels, ppu_draw_buffer, 0xf000);

}
#endif
byte ppu3::DEBUG_EXPOSE_REGISTER(unsigned short reg)
{
    if(reg==0x2000)
        return reg_2000;

                return 0;
}


















void ppu3::fill_tmp_sprbuf()
{
  //  memset(tOAM, 0, sizeof(OAM_struct) * 0x40);
    byte xpos, ypos;
    byte attr, indx;

    byte *OAMptr;

    word n;

    sprites_in_range = 0;
    byte sprite_height = 8 + ((reg_2001 & 0x20) >> 2); // 8 or 16 pixels

    word offset = 0;
    word scan = current_scanline - 21;

    for(int i=0;i<64;i++)
    {
        OAMptr = &OAM[i<<2];

        ypos = (*OAMptr) + 1;
        if(scan >= ypos && scan <(ypos + sprite_height) )
        {
            if(sprites_in_range < 8) // once we've preloaded 8 sprites, we haffi stop :-o
            {
                this->tOAM[sprites_in_range].draw_x = 0;
                this->tOAM[sprites_in_range].x = OAMptr[3] + this->fine_x;
                this->tOAM[sprites_in_range].attributes = OAMptr[2];
                this->tOAM[sprites_in_range].index = i;//OAMptr[1];
                this->tOAM[sprites_in_range].tile_no = OAMptr[1];
                if(sprite_height == 8)
                    n = (OAMptr[1] * 16) | ((reg_2000 & 8)<<9);
                else
                    n = ((OAMptr[1] &~1)*16) | ((OAMptr[1] & 1 )<<12);

                offset = scan - ypos;
                if(tOAM[sprites_in_range].attributes & 0x80) //vertical flip
                {
                    n += 7;
                    n -= offset;
                    n += (reg_2001 & 0x20) >> 1;
                    n -= (offset & 8);
                }
                else
                {
                    n += offset;
                    n += (offset & 8);
                }

                tOAM[sprites_in_range].tile1 = readptr[(n >> 10) & 7][n&0x3FF];
                n+= 8;
                tOAM[sprites_in_range].tile2 = readptr[(n >> 10) & 7][n&0x3FF];
                sprites_in_range++;


            }
            else
                reg_2000 |= 0x20; //set sprite overflow.
        }

    }

}


/*void ppu3::scanline_sprites_2()
{
    byte x =0;
    byte draw_pixel = 0;

    for(int i=0;i<this->sprites_in_range;i++)
    {
        if(tOAM[i].x == 0) //drawing time?
        {
            tOAM[i].draw_x = 8;

        }
        tOAM[i].x--;
        if(tOAM[i].draw_x > 0)
        {
            tOAM[i].draw_x--;
            x = tOAM[i].draw_x;
            if(tOAM[i].attributes & 0x40) //hflip
                x = 7 - tOAM[i].draw_x;
            if(this->sprite_zero == 0 && tOAM[i].index == 0) //spr0
            {
                draw_pixel = (tOAM[i].attributes & 3) >> 2;
                draw_pixel |= (tOAM[i].tile1 >> x) & 1;
                draw_pixel |= ((tOAM[i].tile2 >> x)&1)<<1;



            }
        }


    }
}*/


word calc(byte val)
{
    if(val == 0)
        return 0x2000;
    if(val==1)
        return 0x2400;
    if(val==2)
        return 0x2800;
    if(val==3)
        return 0x2c00;
    else return 0xffff;
}


void ppu3::SetNameTable(unsigned char nt_num)
{
    byte nt = nt_num & 3;
    byte  n = nt | 8;

    this->readptr[n] = this->writeptr[n] = this->readptr[n+4] = this->writeptr[n+4] = nametables + (0x400 * nt);
}

void ppu3::SetNameTables(unsigned char nt1, unsigned char nt2, unsigned char nt3, unsigned char nt4)
{

}

void ppu3::SetLowerPtrs(unsigned char num, unsigned char *data)
{
    this->readptr[num] = writeptr[num] = (data + (num * 0x400));
}

void ppu3::Step()
{
    byte h,l;
    switch(pixel)
    {

        case 0:

            if(this->current_scanline == 20) //end of vertical interrupt
            {
                reg_2002 &= ~(0x40 | 0x20); //clear sprite zero

            }
            if(this->current_scanline >= 21 && current_scanline <= 261 && SPRITES_ENABLED)
            {
                this->fill_tmp_sprbuf();
            }
            break;

        case 1:
            if(current_scanline == 0 && (reg_2000 & 0x80))
                reg_2002 |= 0x80; //set NMI bit in status register
            break;
        case 2:
         case 4:  case 6:
                     break;



        case 8:        case 16:        case 24:
        case 32:case 40:        case 48:
        case 56:        case 64:
        case 72:        case 80:
        case 88:
        case 96:        case 104:
        case 112:        case 120:
        case 128:        case 136:
        case 144:        case 152:
        case 160:        case 168:
        case 176:        case 184:
        case 192:        case 200:
        case 208:        case 216:
        case 224:        case 232:
        case 240:        case 248:
        if(current_scanline >= 21 && current_scanline <= 261 && BG_SPR_ENABLED)
            {
                     this->pixel_copy(0,1);
                     this->pixel_copy(1,2);
            //ppu copying
                 }
                     break;


case 9:case 17:
case 25:
case 33:case 41:
case 49:case 57:
case 65:case 73:
case 81:case 89:
case 97:case 105:
case 113:case 121:
case 129:case 137:
case 145:case 153:
case 161:case 169:
case 177:case 185:
case 193:case 201:
case 209:case 217:
case 225:case 233:
case 241:case 249:
    {
                if(current_scanline >= 21 && current_scanline <= 261 && BG_SPR_ENABLED)
                {

                    ppu_pixels[2].nametable_entry = readptr[((ppu_addr>>10)&3)+8][ppu_addr & 0x3FF];
                    ppu_pixels[2].chr_addr = (ppu_pixels[2].nametable_entry * 16) + ((reg_2000 &0x10) << 8);
                    ppu_pixels[2].chr_addr += ppu_addr >> 12;

                }
        //fetch next byte

    }
break;

 case 3: case 11:
 case 19: case 27:
 case 35:   case 43: case 51: case 59: case 67:
     case 75:case 83: case 91: case 99: case 107: case 115: case 123: case 131: case 139:
   case 147: case 155:case 163:case 171:case 179:case 187: case 195:

       case 203: case 211: case 219: case 227: case 235: case 243:  case 251:

           //do HT
           if(current_scanline >= 21 && current_scanline <= 261 && BG_SPR_ENABLED)
            {
           byte a;
           unsigned long addr = 0;

           addr = ppu_addr & 0x3C00;
           addr += 0x3c0 +((ppu_addr >>2)&7) + (((ppu_addr >> (2+5))&7)<<3);
           a = readptr[((addr >> 10) & 3) + 8][addr & 0x3FF];
           ppu_pixels[2].attrib = ((a >> (((ppu_addr & 2) | (((ppu_addr >> 5)&2)<<1))))&3)<<2;

           if((ppu_addr & 0x1F) == 0x1f) //did we generate carry?
               ppu_addr^=0x41F; //hell yeah
           else
               ppu_addr++;

            }
           break;


       case 5:case 13:case 21: case 29:case 37: case 45: case 53:case 61:case 69: case 77:
        case 85: case 93:  case 101: case 109: case 117: case 125:
            case 133: case 141:  case 149:
       case 157:
           case 165: case 173: case 181: case 189: case 197:  case 205: case 213:  case 221: case 229:
                       case 237: case 245:  case 253:
                           if(current_scanline >= 21 && current_scanline <= 261 && BG_SPR_ENABLED)
                            {
                                    ppu_pixels[2].tile_data[0] = readptr[(ppu_pixels[2].chr_addr >> 10)&7][ppu_pixels[2].chr_addr & 0x3FF];

                            }
                           //every day I'm pipelinin'
                           break;
          case 7:case 15: case 23:
        case 31: case 39: case 47: case 55: case 63:  case 71:
         case 79: case 87: case 95:  case 103: case 111:
           case 119:  case 127:  case 135:
               case 143: case 151:  case 159:   case 167: case 175:
                   case 183:case 191: case 199:case 207: case 215:  case 223: case 231:   case 239: case 247: case 255:

                       if(current_scanline >= 21 && current_scanline <= 261 && BG_SPR_ENABLED)
                           {
                                word p = ppu_pixels[2].chr_addr + 8;
                                ppu_pixels[2].tile_data[1] =readptr[(p>>10)&7][p&0x3FF];
                            }
                       break; //second nt tile
              /*


case 10:
    case 12:  case 14:
       case 18:
    case 20: case 22:
    case 26:  case 28:  case 30:
     case 34:  case 36:  case 38:
       case 42:  case 44:
    case 46:    case 50: case 52:
    case 54:   case 58:
    case 60:  case 62:  case 66:
  case 68:  case 70:
    case 74:  case 76:  case 78:
    case 82: case 84: case 86:
      case 90:  case 92:  case 94:
      case 98: case 100:
    case 102:    case 106:  case 108:
    case 110: case 114:  case 116:
     case 118:   case 122:
    case 124: case 126:   case 130:
case 132:  case 134:
    case 138:      case 140: case 142:
     case 146: case 148: case 150:
      case 154:  case 156: case 158:
    case 162:  case 164:
    case 166:    case 170:  case 172:
   case 174:    case 178:
    case 180:   case 182:   case 186:
      case 188:  case 190:
    case 194:  case 196: case 198:
     case 202:  case 204:  case 206:
     case 210: case 212: case 214:
    case 218: case 220:
    case 222:    case 226:  case 228:
     case 230:   case 234:
    case 236:  case 238: case 242:
  case 244: case 246:  case 183:case 191: case 199:case 207: case 215:  case 223: case 231:   case 239: case 247:
    case 250:
      */

case 252:
            {
                if(current_scanline == 20)
                {
                    reg_2002 = 0;
                    reg_2003 = 0;//reset OAM pointer

                    if(BG_SPR_ENABLED)
                        this->ppu_addr = this->ppu_temp;

                }
            }
              break;

//case 254:

case 256:
             if(current_scanline >= 21 && current_scanline <= 261 && BG_SPR_ENABLED )
            {
            this->ppu_addr &= ~0x41F;
            ppu_addr |= ppu_temp & 0x41F;
            if((ppu_addr & 0x7000) == 0x7000)
            {
                int t = ppu_addr & 0x3E0; //
                ppu_addr &= 0xFFF;
                if(t == 0x3A0)
                {
                    ppu_addr ^= 0xBA0;
                }
                else if(t==0x3E0)
                {
                    ppu_addr ^=0x3E0;
                }
                else
                    ppu_addr+=0x20;
            }
            else
                ppu_addr += 0x1000;
        }
     break;

/*
    case 257: case 258: case 259: case 260: case 261: case 262: case 263:
    case 264: case 265: case 266: case 267: case 268: case 269: case 270:
    case 271: case 272: case 273: case 274: case 275: case 276: case 277:
    case 278: case 279: case 280: case 281: case 282: case 283: case 284:
    case 285: case 286: case 287: case 288: case 289: case 290: case 291:
    case 292: case 293: case 294: case 295: case 296: case 297: case 298:
    case 299: case 300: case 301: case 302: case 303: case 304: case 305:
    case 306: case 307: case 308: case 309: case 310: case 311: case 312:
    case 313: case 314: case 315: case 316: case 317: case 318: case 319:
    case 320:  case 322:  case 324:  case 326:
     case 330:  case 332:
    case 334:case 337: case 338: case 339: case 340:
    case 341: case 342: case 343: case 344: case 345: case 346: case 347:
    case 348: case 349: case 350: case 351: case 352: case 353: case 354:
    case 355: case 356: case 357: case 358: case 359:

*/
case 329:
    case 321:
     if(current_scanline >= 20 && current_scanline <= 261 && BG_SPR_ENABLED)
     {
         ppu_pixels[2].nametable_entry = readptr[((ppu_addr >> 10) & 3) + 8][ppu_addr & 0x3FF];
         ppu_pixels[2].chr_addr = (ppu_pixels[2].nametable_entry * 16) + ((reg_2000 & 0x10)<< 8);
         ppu_pixels[2].chr_addr += ppu_addr >> 12;
     }
    //start playfield pipe
    break;


    case 331:
    if(current_scanline == 19)
        //VINT PINISH!
        reg_2002 &= ~0x80;
    //break; //<---?

    case 323:
    if(current_scanline >= 20 && current_scanline <= 261 && BG_SPR_ENABLED)
    {
        byte a;
        word addr;
        addr = ppu_addr & 0x3C00;
        addr += 0x3c0 + ((ppu_addr >> 2) & 7) + (((ppu_addr >> (2+5))&7)<<3);
        a = readptr[((addr >> 10) & 3) + 8][addr & 0x3FF];
        ppu_pixels[2].attrib = ((a >> (((ppu_addr & 2) | (((ppu_addr >> 5)&2)<<1))))& 3)<<2;
        if((ppu_addr & 0x1F) == 0x1F)
            ppu_addr ^= 0x41F;
        else
            ppu_addr++;

    }
    break;

    case 325:case 333:

                //do pipestuff

                if(current_scanline >= 20 && current_scanline <= 261 && BG_SPR_ENABLED)
                {
                    ppu_pixels[2].tile_data[0] = readptr[(ppu_pixels[2].chr_addr >> 10) & 7][ppu_pixels[2].chr_addr & 0x3FF];
                }

                    break;

    case 327: case 335:
                if(current_scanline >= 20 && current_scanline >= 261 && BG_SPR_ENABLED)
                    {
                        word p = ppu_pixels[2].chr_addr + 8;
                        ppu_pixels[2].tile_data[1] = readptr[(p >> 10) & 7][p & 0x3FF];
                     }
                break;


    case 328:
                if(current_scanline >= 20 && current_scanline <= 261 && (reg_2001 & 8))
                {
                    //HERE BE SOME MOTHERFUCKING DRAGONS
                    pixel++;
                    Step();
                    pixel--;

                }


         case 336: //do copying
             if(current_scanline >= 20 && current_scanline <= 261 && BG_SPR_ENABLED)
             {
                 pixel_copy(0,1);
                 pixel_copy(1,2);
             }




 case 337: case 338: case 339:
                         break;

     case 340: //last pixel in the world :-(

         if( (current_scanline >= 21) && (current_scanline<=260))
         {

        /*     int ss = current_scanline - 21;
             memcpy(this->draw_surface + (ss*256), this->scanline_pixels, 256);//(this->scanline_pixels + fine_x), (256-fine_x));
         */}
         if(current_scanline == 261)
         {
             if(this->even_odd_frame)
                 even_odd_frame = 0;
             else
                 even_odd_frame = 1;

             reg_2002 |= 0x80;
             current_scanline = 0;
             this->sprite_zero = false;
             this->frames_drawn ++;
             draw=true;
         }
         else
         {
             current_scanline++;
         }


    pixel = 0;

    //it's ok to waste cycles here if appropriate...
    return;


        default:break;

    }

    if(current_scanline >= 21 && current_scanline <= 261 && pixel < 256)
    {
        //draw some motherfucking pixels!
        word scanline = current_scanline - 21;
        byte pxpos =  7-(pixel & 7) ;
        byte bg = 0;
        byte sp = 0;
        h = 8 << ((reg_2000 >> 5) & 1);
        if(reg_2001 & 0x8)
        {
            bg = ppu_pixels[0].attrib;
            bg |=  (ppu_pixels[0].tile_data[0] >> pxpos) & 1;
            bg |= ((ppu_pixels[0].tile_data[1] >> pxpos)&1)<<1;

        }
        if(reg_2001 & 0x10)
        {
            for(int i=0;i<sprites_in_range;i++)
            {
                if(tOAM[i].x == 0)
                {
                    tOAM[i].draw_x = 8;
                }
                tOAM[i].x --;
                if(tOAM[i].draw_x >0)
                {
                    tOAM[i].draw_x--;
                    pxpos = tOAM[i].draw_x;

                    if(tOAM[i].attributes & 0x40)
                    //hflip
                    {
                        pxpos = 7 - tOAM[i].draw_x;
                    }
                    if(this->sprite_zero == 0 && tOAM[i].index == 0)
                    {
                        sp = (tOAM[i].attributes & 3) <<2;
                        sp |= (tOAM[i].tile1 >> pxpos) & 1;
                        sp |= ((tOAM[i].tile2 >> pxpos) & 1) << 1;
                        if((bg & 3) && (sp & 3))
                        {
                            if(((reg_2001 & 0x1E) != 0x1E) && (pixel < (8+fine_x)) || pixel == 255)
                            {
                                //sprite zero was clipped!
                            }
                            else
                            {
                                reg_2002 |= 0x40;
                                sprite_zero = true; //sprite zero hit!
                            }
                        }
                    }
                    if(tOAM[i].attributes & 0x20)
                    {
                        //sprite behind BG?
                        if((bg&3)==0)
                        {
                            sp = (tOAM[i].attributes & 3) << 2;
                            sp |= (tOAM[i].tile1 >>pxpos) & 1;
                            sp |= ((tOAM[i].tile2 >>pxpos)&1)<<1;
                        }
                    }

                    else
                    {
                        sp = (tOAM[i].attributes & 3) << 2;
                        sp |= (tOAM[i].tile1 >> pxpos) & 1;
                        sp |= ((tOAM[i].tile2 >> pxpos)&1) <<1;
                    }
                    if(sp & 3)
                        bg = sp | 0x10;
                }
            }
        }


       if(reg_2001 & 1) //b&w
           bg &= 0x30;
       if((reg_2001 & 0x1E) != 0x1E && (pixel < (8+fine_x)))
           sp = 0;
       if((reg_2001 & 2) == 0 && (pixel < (8+fine_x)))
           this->draw_surface[(unsigned short) (scanline * 256)+pixel] = palette[sp|0x10];
           //this->scanline_pixels[pixel] = palette[sp | 0x10];
       else
           this->draw_surface[(unsigned short) (scanline * 256)+pixel] = palette[bg];

           //scanline_pixels[pixel] = palette[p];


    }
    pixel++;

}

void ppu3::RunCycles(int cycles)
{
for(int i=0;i<cycles;i++)
    Step();
}

inline void ppu3::pixel_copy(unsigned char to, unsigned char from)
{
    this->ppu_pixels[to].attrib = this->ppu_pixels[from].attrib;
    this->ppu_pixels[to].nametable_entry = this->ppu_pixels[from].nametable_entry;
    this->ppu_pixels[to].tile_data[0] = this->ppu_pixels[from].tile_data[0];
    this->ppu_pixels[to].tile_data[1] = this->ppu_pixels[from].tile_data[1];

}

inline byte ppu3::ReadInternal(unsigned short addr)
{

    word offset = (addr & 0x3fff) >> 10;


    if(this->readptr[offset])
            return(this->readptr[offset][addr&0x3ff]);

return 0;
}

void ppu3::WriteInternal(unsigned short addr, unsigned char data)
{
    word offset = (addr & 0x3FFF) >> 10;
    if(this->writeptr[offset])
        writeptr[offset][addr & 0x3ff] = data;

}

