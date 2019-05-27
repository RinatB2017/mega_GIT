//#include "stdafx.h"

//#include "cpu.h"
#include "ppu.h"
#include "famicom.h"
#include "palette.h"
#include <time.h>
#include <assert.h>

time_t timer;



PPU::PPU()
{

        //memory = (byte *)malloc(0x4000);
	memset(memory, 0, 0x4000);

	ppu_ticks = 0;
	ppu_adress = 0;


	memset(ppu_draw_buffer,0,0xf000);

	num_scanline = 0;
	VBL_occured = false;
	NMI_occured = false;

	
	memset(DEBUG_TILE_DATA, 0, 8*8);
	
	

	

	memset(OAM, 0, 0xFF);
	//bitmap = 0;

	timer = time(NULL);


        ppu_bus = 0;
	reg_2007_first_read = 0;


	sprite_zero = 0;
	memset(sprite_zero_buffer, 0, 256);

	scroll_x = scroll_y = 0;
	toggle_2006 = toggle_2005 = 0;
	sprite_overflow = 0;
	word ppu_adress_prev=0;




	reg_2000 = 0;
	reg_2001 = 0;
	reg_2002 = 0;
        reg_2003 = 0;
	reg_2004 = 0;
	reg_2005 = 0;

//	this->toggle_2006 = 0;

	this->ppu_adress = 0;


        this->NES_PALETTE = SetupPalette();

        DEBUG_X = 0; DEBUG_Y =0;//391; //-218;


 //	memptr[0x2002] = SET_BIT(memptr[2002], 7);
}

PPU::~PPU()
{
    delete [] NES_PALETTE;

	
}


void PPU::VBlank()
{


//now we're in vblank! do screen updates...




}

void PPU::Update()
{
	do_scanline();
	ppu_ticks++;
}
bool PPU::end_of_frame()
{ 
		if(ppu_ticks >= 357368)
		{
			ppu_ticks=0; 
			return true;
		}
			
			return false;
}

byte PPU::read_ppu(word adress)
{
        byte ret_byte = ppu_bus;
	//printf("ppu read @ $%04x\n", adress);
	
	switch(adress)
	{


	case 0x2002: //reading this register clears PPUscroll and the adress latch!!
                ret_byte = reg_2002 | (ppu_bus & 0x1f);
	
		CLR_BIT(reg_2002, 7);
                toggle_2006 = 0 ; //clear adress latch $2005/$2006



		return ret_byte;
		break;

	case 0x2004:
		return OAM[reg_2003]; //read OAM with offset in reg_2003 (previously set :-))
           //     reg_2003++;
		break;

	case 0x2007: //read from adress, previously set by two 0x2006-writes
			//ugly hack to avoid bad memory access due to buggy PPU implementation
		//	if(ppu_adress<0x20fa)



            ret_byte = ppu_bus;

            ppu_bus = memory[ppu_adress & 0x3FFF];


                if(ppu_adress>0x3f1F && ppu_adress <= 0x3FFF)
                {


                    ppu_bus= memory[ppu_adress & 0x3f1f]; //palette mirror from 0x3f1f - 0x3fff

                }





                ppu_adress += (GET_BIT(reg_2000, 2)?32:1);




                ppu_adress &= 0x3fff;


		return ret_byte;
			

	
//		//printf("ppu->cpu@$%04x[ppu]=$%02x\n", ppu_adress, memory[ppu_adress]);
		break;


	default: break;
	}

	return 0;
}

void PPU::write_ppu(word adress, byte val)
{
	switch (adress)
	{
	case 0x2000:
	//	NMI_at_VBLANK = val&0x80?1:0; // see if we need to set NMI to occur during frame refresh (vblank)
		
	//incrementation is done elsewere, dumbass	//ppu_adress += GET_BIT(val, 2) ?32:1; 

		reg_2000 = val;
	break;

	case 0x2001:
		reg_2001 = val;
		break;
	case 0x2002:
		reg_2002 = val;
	break;

	case 0x2003:
		reg_2003_dma_counter = val;
		reg_2003 = val;
	break;

	case 0x2004:
		OAM[reg_2003_dma_counter] = val;
		reg_2003_dma_counter ++; //increment Sprite ram offset (reg $2003) every write to $2004
	break;

	case 0x2005:
                if(!this->toggle_2006) //no value has been written previously (same latch as $2006 (yes?)
		{
                        scroll_y = val;
                        this->toggle_2006 = true;

		
		}
		else
		{
                        scroll_x = val;


                        if(scroll_x > 239)
                        {
                                scroll_x =0;
                        }
                        toggle_2006 = false;
		}
		//reg_2005 = val; //SCROLL
	break;
	case 0x2006: //set adress for memory access

 		if(!this->toggle_2006)
		{
			ppu_adress_prev = ppu_adress;
                        ppu_bus = val;

			toggle_2006 = true;
		}
		else
		{
                        ppu_adress = MAKE_WORD(val, this->ppu_bus); //memory access adress
			toggle_2006 = false;

                        if(ppu_adress >= 0x3f00 && ppu_adress <=0x3fff) //don't discard the first byte read from 2007 when we're reading from the palette + mirrors...
				reg_2007_first_read = false;
			else
                            reg_2007_first_read = true;

			if( (ppu_adress_prev==0) && (this->num_scanline < 240))
			{

				//ppu scrolling trick in progress? stole this trick from somewhere of the internets :)

				//works in nestress.nes though :)
				if( ppu_adress >= 0x2000 && ppu_adress <= 0x2400)
				{
					this->scroll_x = ((ppu_adress - 0x2000)/0x20) * 8 - num_scanline-1;
				}
			}
			
		
                        ppu_bus = val;
		}


		break;

	case 0x2007:






/*  the LSB of the last byte written in to the PPU goes in to the LSB of $2002
  */






                if(ppu_adress < 0x2000) //sprite and nametable data
			memory[ppu_adress] = val;
	
		else if(ppu_adress>0x3f20) //mirrors
		{

		//	printf("[PPU.cpp]mirroring @ $%04X!->$%04X\n", ppu_adress, ppu_adress&0x3FFF);
			memory[ppu_adress&0x3FFF] = val; //mirroring... 
			
		}


                else if(ppu_adress >=0x2000 && ppu_adress<=0x3EFF) //do name table mirroring
		{
                    if(ppu_adress >0x3000 && ppu_adress <= 0x3EFF)//nametable mirroring
                          ppu_adress -= 0x1000;




/* TODO: MIRRORING, SCROLLING */

			if(vertical_mirroring)
			{

				switch(ppu_adress & 0x2C00)
				{
				case 0x2000:
					memory[ppu_adress] = val;
					break;
				case 0x2400:
					memory[ppu_adress] = val;
					break;
				case 0x2800:
					memory[ppu_adress-0x800] = val;
					break;
				case 0x2C00:
					memory[ppu_adress-0x800] = val;
					break;
				}
			}

			 if(horizontal_mirroring)
			{
				switch(ppu_adress & 0x2C00)
				{
				case 0x2000:
					memory[ppu_adress] = val;
					break;
				case 0x2400:
					memory[ppu_adress-0x400] = val;
					break;
				case 0x2800:
					memory[ppu_adress-0x400] = val;
					break;
				case 0x2C00:
					memory[ppu_adress-0x800] = val;
					break;
				}
				
			}



			//memory[ppu_adress] = val;
			
			
		//		//printf("wrote $%02X @ $%04X\n", val, ppu_adress);
		}

                else if(ppu_adress>=0x3f00 && ppu_adress<=0x3fff)
                {


			/*	switch(ppu_adress)
				{
				case 0x3f10: ppu_adress = 0x3f00;
					break;
				case 0x3f14: ppu_adress = 0x3f04;
					break;
				case 0x3f18: ppu_adress = 0x3f08;
					break;
				case 0x3f1c: ppu_adress = 0x3f0c;
					break;
				}
			*/

                                memory[ppu_adress &0x3f1f]=val;
                                if(!((ppu_adress &0x3f1f)&0x7))
                                        memory[((ppu_adress&0x3f1f)^0x10)] = val;
                }



		ppu_adress += GET_BIT(reg_2000, 2)? 32:1; //increment by 32 if bit is set, 1 if NULL



                ppu_adress &= 0x3FFF; //align

		break;


        default:
            break;
	}

}


bool PPU::isSpriteOnScanline(int scanline, int y)
{
 if(y < scanline-20 && (y+8) > scanline-20)
	 return true;
 return false;
}


void PPU::draw_background_2_0(int nScanline)
{



}

void PPU::draw_background_on_scanline(int nScanline)
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

	switch(reg_2000 & 0x3)
	{
	case 0:
		name_table_offset = 0x2000;
		break;
	case 1:
		name_table_offset = 0x2400;
		break;
	case 2:
		name_table_offset = 0x2800;
		break;
	case 3:
		name_table_offset = 0x2C00;
		break;
	default:break;
	}


	
	//get offset in the name table data...
	









	byte col = 0;
	byte tile1 = 0; // the tile data
	byte tile2 = 0;

	byte tile_x_limit = 0;
	byte tile_x_start = 0;
	
 int scroll_scanline = (num_scanline-1) + scroll_x;
	
	for(int scroll_side = 0; scroll_side < 2; scroll_side++)
	{
		
	   	name_table_base = name_table_offset;

		if(scroll_side == 0)
		{
			if(scroll_scanline >= 240)
			{
				switch(name_table_offset)
				{
				case 0x2000: 
					name_table_base = 0x2800;
				break;
				case 0x2400:
					name_table_base = 0x2C00;
				break;
				case 0x2800:
					name_table_base = 0x2000;
				break;
				case 0x2C00:
					name_table_base = 0x2400;
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
				case 0x2000:
					name_table_base = 0x2c00;
				break;
				case 0x2400:
					name_table_base = 0x2800;
				break;
				case 0x2800:
					name_table_base = 0x2400;
				break;
				case 0x2c00:
					name_table_base = 0x2000;
				break;
				}
				scroll_scanline = scroll_scanline - 240;
			}
			else
			{
				switch(name_table_offset)
				{
				case 0x2000:
					name_table_base = 0x2400;
				break;
				case 0x2400:
					name_table_base = 0x2000;
				break;
				case 0x2800:
					name_table_base = 0x2C00;
				break;
				case 0x2C00:
					name_table_base = 0x2800;
				break;
				}
			}
			tile_x_start = 0;
			tile_x_limit = (scroll_y / 8) + 1;
		}

		

	if(vertical_mirroring)
	{
		switch(name_table_base)
		{
		case 0x2800: 
			name_table_base= 0x2000;
		break;
		case 0x2C00:
			name_table_base = 0x2400;
		break;
		}
	}
	if(horizontal_mirroring)
	{
		switch(name_table_base)
		{
		case 0x2400:
			name_table_base = 0x2000;
		break;
		case 0x2800:
			name_table_base = 0x2400;
		break;
		case 0x2C00:
			name_table_base = 0x2400;
		break;
		}
	}
			

		for(int tile_x = tile_x_start; tile_x < tile_x_limit; tile_x++)

		{

			byte tile_number = memory[name_table_base + ((scroll_scanline/8)*32)+ tile_x];

			word tile_offset = (GET_BIT(reg_2000, 4)? 0x1000 : 0x0000) + (tile_number * 16); //bit 4 in $2000 determines which tile-table is for the BG

			tile1 = memory[tile_offset +(scroll_scanline%8)];

			tile2 = memory[tile_offset +(scroll_scanline%8)+8];
		

			//compute upper two bits of the palette from the attribute table of the current name table

			palette_high = memory[name_table_base + 0x3c0 + (((scroll_scanline/8)/4) * 8) + (tile_x / 4)];

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
						ppu_draw_buffer[draw_offset] = memory[0x3f00+col] & 0x3f;
					//	else
					//		printf("heap error!");
						//do sprite 0 stuff
					
						if(!sprite_zero)
						{
						//	sprite_zero_buffer[(8 * tile_x) - scroll_y + tx] += 4;
						
						}
					}
					else
					{
						if(((8*tile_x)+ (256 - scroll_y) + tx) < 256)
						{
                                                        draw_offset =  (scanline * 256) + (8*tile_x) + (256 - scroll_y)+ tx;
						//	if(draw_offset < 0xf000)
							ppu_draw_buffer[draw_offset] = memory[0x3f00+col] & 0x3f;
						//	else
						//		printf("heap error");
						//do sprite0 stuff...
							if(!sprite_zero)
							{
						//		this->sprite_zero_buffer[(8*tile_x)+(256-scroll_y)+tx]+=4;
					
							}
						}
					}
				}

					
					
					
				

				//	ppu_draw_buffer[(scanline * 256) + (8*tile_x) + tx] = 0x3f& memory[0x3f00 + (col)];

				//	 else

				//assume background

				//		 ppu_draw_buffer[(scanline*256)+(8*tile_x)+tx] = memory[0x3f00];
		

			}




		}

	}
}


void PPU::draw_sprites_new(int nScanline, bool front)
{

    byte oam_x, oam_y, oam_attr, oam_num;


    word offset = 0;

    bool sprite_size = (reg_2000 & 0x20); //sixth bit denotes sprite size

    byte t1, t2;
    byte col_high_bits;
    byte col;

    int scanline = nScanline;

    for(int i=252; i>=0; i-=4)
    {
        oam_x = OAM[i+3];
        oam_attr = OAM[i+2];
        oam_num = OAM[i+1];
        oam_y = OAM[i]+1;




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



            t1 = memory[offset];
            t2 = memory[offset+8];

            for(int sx=0;sx<8;sx++)
            {

                if((oam_attr & 0x40) == 0x40) //flip horizontally?
                     col = col_high_bits + (((t2 & (1 << (sx))) >> (sx)) << 1) +((t1 & (1 << (sx))) >> (sx));

                else
                      col = col_high_bits + (((t2 & (1 << (7 - sx))) >> (7 - sx)) << 1) +((t1 & (1 << (7 - sx))) >> (7 - sx));



                if((col&1)||(col&2)) //every four palette byte is zero :-o
                   {
                       if( (oam_x +sx) < 256)
                       {

                          ppu_draw_buffer[scanline * 256 + oam_x +sx] = memory[0x3f10 + col];


                       }

                       //if we have a non-zero pixel and we're drawing sprite zero, set the flag
                       if(i==0)
                          this->sprite_zero = 1;

                   }



          }



      }
    }
}





void PPU::draw_sprites_on_scanline(int nScanline, int magic)
{
	int scanline = nScanline;
	int oam_y = 0;
	int oam_x = 0;
	int sprite_y =0;
	int sprite_x = 0;
        byte sprite_num = 0;
	int offset = 0;
	byte tile1 = 0;
	byte tile2 = 0;
	byte palette_high = 0;
	byte col = 0;

	int sx=0;
	int draw_offset = 0;



	

	 //where are we getting the sprite data from?
		 //where are we getting the sprite data from?
			
	

        for(int iOAM = 252; iOAM>=0; iOAM-=4) //backwards sprite priority in real ppu
	{
		//check if sprite is visible
		
			oam_y = OAM[iOAM]+1;
                        sprite_num = OAM[iOAM+1] ;
                        offset = 0;


			if( ((OAM[iOAM+2] & 0x20) == magic) && ( oam_y <= scanline) && ((oam_y + (GET_BIT(reg_2000, 5)==0?8:16)) > scanline) ) //check if sprite is visible and if it falls on the current scanline
			{
			
				sprite_overflow++;
			
				oam_x = OAM[iOAM+3];
			

			
			//are we drawing 8x8 sprite(s)?
                       if(GET_BIT(reg_2000, 5)==0) //if 0, 8x8 else, 8x16
			{


                                if(GET_BIT(OAM[iOAM+2], 7) == 1) //is the sprite flipped vertically?
					//no
					sprite_y = scanline - oam_y;
                                else
					//yes
                                        sprite_y = oam_y + (7 - scanline);

                                offset = (GET_BIT(reg_2000, 3)? 0x1000 : 0x0000) + ( sprite_num * 16);
				
				tile1 = memory[offset + sprite_y];
				tile2 = memory[offset + sprite_y + 8];
			

				palette_high = (OAM[iOAM+2] & 0x3) << 2;

				for(sx=0;sx<8;sx++)
				{
					if( (OAM[iOAM+2] & 0x40) ==0x40) //flip-flop?
						col = palette_high + (((tile2 & (1 << (sx))) >> (sx)) << 1) +
                                       ((tile1 & (1 << (sx))) >> (sx));
					else
						col = palette_high + (((tile2 & (1 << (7 - sx))) >> (7 - sx)) << 1) +
                                      ((tile1 & (1 << (7 - sx))) >> (7 - sx));
                           
                                        if( (col << 4) != 0)
					{
						if( (oam_x + sx) < 256)
						{
							draw_offset = (scanline * 256) + oam_x + sx;
							if(draw_offset < 0xf000)
                                                        ppu_draw_buffer[draw_offset ] = memory[0x3f10 + col];
							else
								printf("heap error");
						}
						if(iOAM==0) //sprite zero
							this->sprite_zero = true;
							//this->sprite_zero_buffer[OAM[iOAM+3] + sx] += 1;//another entry in the sprite zero buffer
					}
				}

			}




				


			
			else //8x16 sprites!
			{

				 
		
                                 if( (OAM[iOAM+2]&0x80) != 0x80) //is the sprite flipped vertically?
                                    //no
                                        sprite_y = scanline - oam_y;
                                else
                                    //yup
                                       sprite_y =  oam_y + (15- scanline);

				
                                if(sprite_y < 8) // draw the upper half
				{
				




                                        if( (sprite_num & 0x1) == 0)
                                                offset = 0x0000 + (sprite_num + DEBUG_X &0xFE)  * 0x10;
					else
                                                offset = 0x1000 + ((sprite_num + DEBUG_X &0xFE)+1) * 0x10;


                                   // offset = 0x0000 + ((sprite_num & 0xFE) * 16);
				}
				else //lower half
				{
                                      sprite_y -=  8 ; //align properly


                                        if( (sprite_num & 0x1) == 0)
                                                offset = 0x0000 + ((((sprite_num + 381 )| 0x1 )) * 16);
					else
                                                offset = 0x1000 + ((((sprite_num + 381)| 0x1)
                                                                    )* 16);

                                  //      offset = 0x1000 + ((sprite_num | 0x1) * 16);

				}

                     //         offset = ((sprite_num & ~1) * 16) | ((sprite_num & 1) << 12);

                         //       offset >>=10;
                                /*
                                if(sprite_num & 0x1)
                                    offset = 0x1000 + (sprite_num ) *16 ;
                                else
                                    offset = 0x0000 + (sprite_num ) * 16;
*/

				tile1 = memory[offset+sprite_y];
                                tile2 = memory[offset+sprite_y+0x8];
			//	tile1 = memory[  ((GET_BIT(reg_2000, 3) ? 0x1000 : 0x0000) + (sprite_num * 16)) + sprite_y];
			//	tile2 = memory[ ((GET_BIT(reg_2000, 3) ? 0x0000 : 0x1000) + (sprite_num * 16)) + sprite_y];
				palette_high = (OAM[iOAM+2] & 0x3) << 2;
				for(sx=0;sx<8;sx++)
				{
					if( (OAM[iOAM+2]&0x40) == 0x40) //sprite flipped horizontally?
						col = palette_high + (((tile2 & (1 << (sx))) >> (sx)) << 1) +
                                       ((tile1 & (1 << (sx))) >> (sx));
					else
						col = palette_high + (((tile2 & (1 << (7 - sx))) >> (7 - sx)) << 1) +
                                       ((tile1 & (1 << (7 - sx))) >> (7 - sx));

                                        if( (col % 4) != 0)
					{
					

						if( (oam_x + sx)<256)
                                                        ppu_draw_buffer[ (scanline * 256) + (oam_x + sx)] = memory[0x3f10 + (col&0x1f)];
						if(iOAM == 0)
						{
							this->sprite_zero = true;
						}

                                            }




					
				}




			}

		}
	
		
	}




	
}



int frames = 0;
void PPU::do_scanline()
{

	bool bg_enabled = GET_BIT(reg_2001, 3);

	bool sprite_enabled = GET_BIT(reg_2001, 4);


	if(num_scanline==0)
	{
            draw = false;
		CLR_BIT(reg_2002, 7); //vblank is over for this time :-c
           //     CLR_BIT(reg_2002, 6);
         //       CLR_BIT(reg_2002, 5);

               // this->toggle_2006 = false;
		
	

	}
	else if(num_scanline < 241)
        {
	




//clear the current scanline and sprite zero buffer
       if(memory[0x3f00] > 63)
	{
		for(int i=0;i<256;i++)
		{
        //		ppu_draw_buffer[((num_scanline-1) *256)+i] = 0;
			sprite_zero_buffer[i] = 0;
		}
        }
	else
	{
		for(int i=0;i<256;i++)
		{
			ppu_draw_buffer[((num_scanline-1)*256)+i] = memory[0x3f00];
			sprite_zero_buffer[i]=0;
		}
	}





	//clear sprite_overflow for our new scanline

		sprite_overflow = 0;
		


		



                if(sprite_enabled)
                    draw_sprites_new(num_scanline-1, false); //draw 'invisible' sprites, slower, but needed for sp0, optimizations can be done :)
                if(bg_enabled)
			draw_background_on_scanline(num_scanline-1);
                if(sprite_enabled)
                    this->draw_sprites_new(num_scanline-1, true); //draw visible sprites

		if(!this->sprite_zero)
		{
			for(int i=0;i<256;i++)
				if(this->sprite_zero_buffer[i] > 4)
					this->sprite_zero = true;
		}


                if(sprite_zero)
			SET_BIT(reg_2002, 6);
		else
			CLR_BIT(reg_2002, 6);




		if(!(reg_2001 & 0x2))//background clipping on?
		{
			for(int i=0;i<8;i++)
				ppu_draw_buffer[((num_scanline-1) * 256) + i] = 0;
		}


		this->sprite_zero_check();

	
	

		
		
		if(sprite_overflow > 7) //more than 8 sprites on the scanline, set the overflow flag in reg_2002
			SET_BIT(reg_2002, 5);
		else
			CLR_BIT(reg_2002, 5);
		//PROBLEM I SPRITERITARENS VÆRLD :c
		

		sprite_zero = 0;

	}

	else if(num_scanline == 241)
	{

		//idle scanline
		

	
	}
	else if(num_scanline==242)
	{
				SET_BIT(reg_2002, 7);//vertical blank started
		//			if(GET_BIT(reg_2002, 6))
				CLR_BIT(reg_2002, 6); //sprite 0 is reset at start of vblank
	
	}

	num_scanline++;
	if(num_scanline == 262)
	{


                //	DEBUG_BLIT_TO_SDL(); //draw the frame
	//		DEBUG_CLEAR_BUFFER();
			num_scanline = 0;

                        draw = true;
	}
	
	


	
}
	

#ifdef IGNORE_FOR_NOW

int frames = 0;
void PPU::do_scanline()
{


	int scanline = 0;
	

		if(GET_BIT(reg_2001, 3))//is bg rendering enabled?
		draw_background_on_scanline(num_scanline);
	
		if(GET_BIT(reg_2001, 4)) //is sprite rendering enabled?
		draw_sprites_on_scanline(num_scanline);
		

		//UGLY SPRITE 0 hack to advance the program :-)
	/*	if(GET_BIT(reg_2002, 6))
		CLR_BIT(reg_2002, 6);
		else
		SET_BIT(reg_2002, 6);
*/
	if(num_scanline==0)
	{
		//do frame timing
		
            //clear $2002



		frames++;
	}

/*	if(num_scanline<=19)
	{
//	draw_background_on_scanline(num_scanline);
	
	
		//do nothing for 20 scanlines, let the cpu do it's thaaaang
	}
*/
	if(num_scanline==20)
	{
//		draw_background_on_scanline(num_scanline);
		//VBL_occured = false;
		CLR_BIT(reg_2002, 7); //end of VBLANK
	}

	if(num_scanline > 20)
	{

	//	scanline = num_scanline - 21; //compute real scanline offset



//	SET_BIT(reg_2002, 6);
		
	
	/*
//	col = 
	word yoff=0;
	byte tilebank = 0;
		//draw nametable
	for(int x_vb=0, x_nt=0;x_vb<256;x_vb++)
	{
		if(x_vb==0)
			DEBUG_GET_TILE(memory[name_table_offset], tilebank, 1);
		tilebank = GET_BIT(reg_2000, 5);
		
		ppu_draw_buffer[x_vb + (num_scanline - 21)*256] = DEBUG_TILE_DATA[x_nt+yoff];
		
		x_nt++;

		if(x_nt>8)
		{
			yoff+=8;
			x_nt=0;

			DEBUG_GET_TILE(memory[name_table_offset], tilebank, 1);
			name_table_offset++;	

		}


		
	}
		

	*/


		//draw shit


	}
	
	num_scanline++;

	if(num_scanline >= 240)
	{
 		num_scanline = 0;
	//	VBL_occured = true;
        //DEBUG_BLIT_TO_SDL(); //draw the frame
        //DEBUG_CLEAR_BUFFER();

		SET_BIT(reg_2002, 7);//vertical blank started
		if(time(NULL) - timer)
		{
			char caption[256];
			char *ptr =(char *) &caption;
			sprintf(ptr, "Famique - FPS: %d", frames);
		    SDL_WM_SetCaption(ptr,"Icon Title");
			timer = time(NULL);
			frames = 0;
		
		}

	}




}

#endif




#define GET_TILE_BYTE(tileindex, num_pixel) (tileindex + c)


void PPU::sprite_zero_check()
{
	byte sx, sy, sprite_num, attrib;

	sy = OAM[0];
	sx = OAM[3];
	sprite_num = OAM[1];
	attrib = OAM[2];

	if(num_scanline >= (sy + (GET_BIT(reg_2000, 5)==1?16:8) -1))
	{
		this->sprite_zero = true;
	}
	else
		this->sprite_zero = false;
	
}


void PPU::DEBUG_CLEAR_BUFFER()
{
     memset(ppu_draw_buffer, 63, 0xf000);


}

void PPU::DEBUG_BLIT_TO_SDL()
{


    Uint32 *ptr;
    int nes_pal_entry = 0;
    int yoffs=0;
    Uint32 *pal32 = ConvertToPalette32(NES_PALETTE);
   _nesRGBStruct *ghetto = ConvertToRGBStruct(NES_PALETTE);
    for(int y =0;y < bitmap->h; y++)
    {
        yoffs = y * bitmap->pitch/4;
        for(int x=0;x <bitmap->w; x++)
        {
        ptr = (Uint32 *)bitmap->pixels + yoffs + x;
        nes_pal_entry = ppu_draw_buffer[x+y*256];
        *ptr = ghetto[nes_pal_entry].r << 16 | ghetto[nes_pal_entry].g <<8 | ghetto[nes_pal_entry].b;

        //NES_PALETTE[nes_pal_entry]<<16 +NES_PALETTE[nes_pal_entry+1] << 8 + NES_PALETTE[nes_pal_entry+2];

        }
    }
   // memcpy(bitmap->pixels, ppu_draw_buffer, 0xf000);

}
