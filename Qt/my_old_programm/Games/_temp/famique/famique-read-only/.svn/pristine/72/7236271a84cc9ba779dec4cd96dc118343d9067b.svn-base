
#include <memory.h>
#include <string.h>
#include "ppu2.h"
#include "ppu/palette.h"

/*

 skriv om read/write & scanline... det behøvs...*/


ppu2::ppu2()
{


    cycles_internal = 0;
    ppu_bus = ppu_address = 0;
    this->current_scanline = 0;

    this->nametables = new byte [0x800];
    memset(nametables, 0, 0x800);


    this->draw_surface = new byte [0xf000];

    this->palette = (byte *)new byte [0x20];
    memset(palette, 0, 0x20);

    memset(this->OAM, 0x00, 0xFF);



    latch = false;

    draw = false;

    reg_2000 = 0;
    this->reg_2001 = 0;
    this->reg_2002 = 0;
    this->reg_2003 = 0;



     this->NES_PALETTE = SetupPalette();
     ppu_cycles = 0;

}

ppu2::~ppu2()
{

    delete [] draw_surface;
    delete [] nametables;
    //cart  = 0;
    delete [] NES_PALETTE;
    delete [] palette;
    //

}


bool ppu2::inVBlank()
{

    return (bool)GET_BIT(reg_2002, 7)==1 ? true:false;


}

bool ppu2::NMI_in_VBlank()
{
    return (bool)GET_BIT(reg_2000, 7)==1 ? true:false;
}



byte ppu2::ReadMemory(word adress)
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




    if(ppu_address < 0x3F00)
    {


        ret_byte = ppu_bus;

    if(ppu_address < 0x2000)
    {
        ppu_bus =  cart->mROMMapper->ReadCHR(ppu_address);//cart->mROMMapper->CHRPtr[ppu_address];
    }




    else if(ppu_address >=0x2000 && ppu_address < 0x3f00)
    {

        if(ppu_address>=0x3000)
            temp_address = (ppu_address-0x1000 - 0x2000) & 0x7FF;
        else temp_address = (ppu_address - 0x2000) & 0x7FF;
        //else
          //  temp_address = ppu_address - 0x2000;

          //  temp_address = (ppu_address - 0x2000 ) &0x7FF;


        //either a read from the nametables or one of its mirrors
        ppu_bus = nametables[temp_address];

    }

}






   if(ppu_address>=0x3f00 && ppu_address <0x3fff)
   {
            ret_byte = palette[(ppu_address & 0x1f)];
         //   ret_byte = ppu_bus;

            /* this is some crazy ass shit right here;
               refer to http://nesdev.parodius.com/bbs/viewtopic.php?t=3554&postdays=0&postorder=asc&start=15&sid=f4b98bcfbcba352399f102431cebef70
               blargg and disch explains it fairly well :)
            */
            ppu_bus = nametables[(ppu_address & 0x7ff)];
   }





        ppu_address += (GET_BIT(reg_2000, 2)? 32:1);




        ppu_address &= 0x3fff;




       break;

    default:
    printf("PPU read out of bounds [0x%04X", adress);
    break;

    }


    return ret_byte;

}



void ppu2::WriteMemory(word adress, byte val)
{

    /* need to bring this function up to date */

    word temp_address;

    switch(adress)
    {

    case 0x2000:
        reg_2000 = val;
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

//    ppu_bus = val;
       if(!latch)
        {
           scroll_y = val;
           latch =true;
       }
       else
       {
           scroll_x = val;

           if(scroll_x > 239)
               {
                   scroll_x = 0;
               }

            latch = false;

       }
       break;

     case 0x2006:
       if(!latch)
       {
           ppu_address_prev = ppu_address;

           //ppu_bus = val;
           this->temp_register = val;
           latch = true;
       }
       else
       {
           ppu_address = MAKE_WORD(val, temp_register);
           latch = false;
           //ppu_bus = 0; //THIS MIGHT BREAK SHIT

           if(ppu_address == 0)
           {
               CLR_BIT(reg_2000, 0);
              CLR_BIT(reg_2000, 1); //writing zero twice to $2006 clears the nametablebits of $2000, aaarg, I need to do a per-pixel renderer!!!
              // ppu_bus = val;
           }
           if( (ppu_address_prev==0) && this->current_scanline<240)
           {
               if(ppu_address >=0x2000 && ppu_address <= 0x2400)
                   scroll_x = ((ppu_address-0x2000)/0x20) * 8 - this->current_scanline -1;


           }
      //     ppu_bus = val;
       }




    break;


    case 0x2007:

    if(ppu_address < 0x2000) /*
                               CHR ROM (R/O)
                               CHR RAM (R/W)
 */  cart->mROMMapper->WriteCHR(ppu_address, val); //        cart->mROMMapper->CHRPtr[ppu_address] = val;

  else  if(ppu_address >= 0x2000 && ppu_address < 0x3f00)
    {
     /*   if(ppu_address >= 0x3000)
            ppu_address &= 0x2fff;
*/

        if(cart->mROMMapper->bHorizontal)
        {
            switch(ppu_address & 0x2C00)
            {
            case 0x2000:

                nametables[(ppu_address-0x2000)] = val;

                break;

            case 0x2400:

                nametables[(ppu_address-0x2000)-0x400] = val;

                break;

            case 0x2800:

                nametables[(ppu_address-0x2000)-0x400] = val;

                break;


            case 0x2C00:

                nametables[(ppu_address-0x2000)-0x800] = val;

                break;
            default:

                printf("PPU vertical mirror below bounds");
                 break;

            }
        }

       else if(cart->mROMMapper->bVertical)
        {
            switch(ppu_address & 0x2C00)
            {
            case 0x2000:
                nametables[ppu_address-0x2000] = val;

                break;

            case 0x2400:
                nametables[ppu_address-0x2000] = val;

                break;

            case 0x2800:
                nametables[(ppu_address-0x2000)-0x800] = val;

                break;

            case 0x2C00:
                nametables[(ppu_address-0x2000)-0x800] = val;

                break;

            default:
                printf("PPU vertical mirror below bounds");
                break;

            }
        }

     else   if(cart->mROMMapper->bSingle)
        {
            if(cart->mROMMapper->wSingleMirroringBase == 0)
            {
                switch(ppu_address & 0x2c00)
                {
                case 0x2000:
                    nametables[ppu_address-0x2000] = val;
                    break;
                case 0x2400:
                    nametables[ppu_address-0x2400] = val;
                    break;
                case 0x2800:
                    nametables[ppu_address-0x2800] = val;
                    break;
                case 0x2c00:
                    nametables[ppu_address-0x2c00] = val;
                    break;

                }

            }
            else if(cart->mROMMapper->wSingleMirroringBase == 0x400)
            {
                switch(ppu_address & 0x2c00)
                {
                case 0x2000:
                    nametables[ppu_address+0x400] = val;
                    break;
                case 0x2400:
                    nametables[ppu_address-0x2000] = val;
                    break;
                case 0x2800:
                    nametables[ppu_address-0x2400] = val;
                    break;
                case 0x2c00:
                    nametables[ppu_address-0x2800] = val;
                    break;

                }
            }


        }
  //      nametables[ppu_address-0x2000] = val;

    }

    else if(ppu_address>=0x3f00 && ppu_address<=0x3fff)
    {




       //palette + mirrors
/*
                    nametables[ppu_address &0x1f1f]=val;
                    if(!((ppu_address &0x1f1f)&0x7))
                            nametables[((ppu_address&0x1f1f)^0x10)] = val;
*/
        palette[ (ppu_address-0x3f00)&0x1f] = val;
   /*     if(!((ppu_address & 0x1f1f) & 0x7))
            palette[((ppu_address-0x3f00)&0x1f)^0x10] = val;
*/
        if(!((ppu_address & 0x1f) & 0x7))
            palette[((ppu_address & 0x1f)^0x10)] = val;



}



    ppu_address += GET_BIT(reg_2000, 2)? 32:1; //increment by 32 if bit is set, 1 if NULL



    ppu_address &= 0x3FFF; //align

    break;


default:break;


    }





}


void ppu2::spr0check()
{

    if(this->current_scanline >= (OAM[0] +(GET_BIT(reg_2000, 5)==1?16:8)-1))
        sprite_zero = true;
    else
        sprite_zero = false;


}
//#define NEW_TEST_PPU

void ppu2::do_scanline()
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





}

/*void ppu2::new_background(int nScanline)
{




}*/

#define PPU_CLOCK_PER_HLINE 341
void ppu2::UGLY_HAX_PPU_CATCH_UP(word num_cpu_cycles)
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
}

void ppu2::scanline_background(int nScanline)
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

void ppu2::scanline_sprites(int nScanline, bool front)
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
            {
                /* disch @ nesdev posted this */

               byte tempY = scanline - oam_y;
               if(tempY < 16)//in range
                   if(oam_attr &0x80)//vertical flip?
                       tempY ^= 15;
               byte tile = oam_num & 0xFE;

               if(tempY&8)
                   tile++;
               tempY&=7;

               /*tile banks differ from 8x16 and 8x8 sprites
                 if 8x8, tilebank is is taken from $2000
                 if 8x16, tilebank is taken from first bit of oam_num
                 */
               offset = (oam_num & 1) ? 0x1000: 0;

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
}


#ifdef IGNORE_FOR_NOW
void ppu2::BlitSDL(SDL_Surface *src)
{

    Uint32 *ptr;
    int nes_pal_entry = 0;
    int yoffs=0;
    Uint32 *pal32 = ConvertToPalette32(NES_PALETTE);
   _nesRGBStruct *ghetto = ConvertToRGBStruct(NES_PALETTE);
    for(int y =0;y < src->h; y++)
    {
        yoffs = y * src->pitch/4;
        for(int x=0;x <src->w; x++)
        {
        ptr = (Uint32 *)src->pixels + yoffs + x;
        nes_pal_entry = draw_surface[x+y*256];
        *ptr = ghetto[nes_pal_entry].r << 16 | ghetto[nes_pal_entry].g <<8 | ghetto[nes_pal_entry].b;

        //NES_PALETTE[nes_pal_entry]<<16 +NES_PALETTE[nes_pal_entry+1] << 8 + NES_PALETTE[nes_pal_entry+2];

        }
    }
   // memcpy(bitmap->pixels, ppu_draw_buffer, 0xf000);

}
#endif

byte *ppu2::UpdateTexMem()
{

    unsigned int *ptr;
  int nes_pal_entry = 0;
  int yoffs = 0;
  //unsigned int *pal32 = ConvertToPalette32(NES_PALETTE);
  _nesRGBStruct *ghetto = ConvertToRGBStruct(NES_PALETTE);
  for(int y=0;y<240; y++)
  {
      yoffs = y * 256;
      for(int x =0; x<256;x++)
      {
          ptr = (unsigned int *)this->texmem + yoffs + x;
          nes_pal_entry = this->draw_surface[x+y*256];
          *ptr = ghetto[nes_pal_entry].r << 16 | ghetto[nes_pal_entry].g <<8 | ghetto[nes_pal_entry].b;

      }
  }
return texmem;
}

byte ppu2::DEBUG_EXPOSE_REGISTER(unsigned short reg)
{
    if(reg==0x2000)
        return reg_2000;

                return 0;
}



void ppu2::RunCycles(int cycles)
{
    this->cycles_internal += cycles / 3;
    run_again:

    if(cycles_internal > NTSC_CYCLES_PER_HBLANK)
    {
        cycles_internal -= NTSC_CYCLES_PER_HBLANK;
        this->do_scanline();
    goto run_again;
    }
    return;
}
