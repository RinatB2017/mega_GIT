// 6502.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
/*

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
*/


//#include <vld.h>

#include "SDL.H"




//#include "cpu.h"
#include "cpu/cpu2.h"
#include "ppu/ppu.h"
#include "ppu/palette.h"
#include "cpu/tracer.h"
#include "nescart.h"


#include <stdio.h>
#include <time.h>
//SDL_Surface *screen;
bool checkSDLEvent(SDL_Event sdlEvent, MemoryManager *mem)
{


	


				if(sdlEvent.type == SDL_QUIT)
				{
					return 1;
					
				}
		
			if(sdlEvent.type == SDL_KEYDOWN)
				{
				 switch(sdlEvent.key.keysym.sym)
				 {
			      case SDLK_LEFT:
					  SET_BIT(mem->control1.controller, 6);
				
			      break;
				  case SDLK_RIGHT:
					  SET_BIT(mem->control1.controller, 7);
					 
				  break;
			      case SDLK_RETURN:
					  SET_BIT(mem->control1.controller, 3);
					 
				  break;
				  case SDLK_UP:
					  SET_BIT(mem->control1.controller, 4);
					
				  break;
	    
				  case SDLK_DOWN:
					  SET_BIT(mem->control1.controller, 5);
					 
				
				  break;

				  case SDLK_a:
					  SET_BIT(mem->control1.controller, 0);
					 
					
					  break;
				  case SDLK_b:
					  SET_BIT(mem->control1.controller, 1);
					

				  case SDLK_SPACE:
					  SET_BIT(mem->control1.controller, 2);
					
					


			     default:
		 
					 break;
				}
			}

			else if(sdlEvent.type == SDL_KEYUP)
			{
					 switch(sdlEvent.key.keysym.sym)
				 {
			          case SDLK_LEFT:
					  CLR_BIT(mem->control1.controller, 6);
				
			      break;
				  case SDLK_RIGHT:
					  CLR_BIT(mem->control1.controller, 7);
					 
				  break;
			      case SDLK_RETURN:
					  CLR_BIT(mem->control1.controller, 3);
					 
				  break;
				  case SDLK_UP:
					  CLR_BIT(mem->control1.controller, 4);
					
				  break;
	    
				  case SDLK_DOWN:
					  CLR_BIT(mem->control1.controller, 5);
					 
				
				  break;

				  case SDLK_a:
					  CLR_BIT(mem->control1.controller, 0);
					 
					
					  break;
				  case SDLK_b:
					  CLR_BIT(mem->control1.controller, 1);
					

				  case SDLK_SPACE:
					  CLR_BIT(mem->control1.controller, 2);
				


			     default:
		 
					 break;
				}
			
			}
			return 0;
}


int main(int argc, char* argv[])
{
	if(argc==1)
		return 1;
	if(argc>1){
		printf("attempting to load %s\n", argv[1]);
	}	

//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	//VLDEnable();
	
	Uint32 initflags = SDL_INIT_VIDEO;  /* See documentation for details */
	SDL_Surface *screen;
	SDL_Surface *draw;
	Uint8  video_bpp = 32;
	Uint32 videoflags = SDL_SWSURFACE;
	int    done;
	SDL_Event event_handler;
	
	/* Initialize the SDL library */
	if ( SDL_Init(initflags) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",
				SDL_GetError());
		exit(1);
	}


	
	
	
	
	/* Set 640x480 video mode */
	screen=SDL_SetVideoMode(256,240, video_bpp, videoflags);
	//SDL_SetColors(screen, screen->format->palette->colors, 0, screen->format->palette->ncolors);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set 640x480x%d video mode: %s\n",
				video_bpp, SDL_GetError());
		SDL_Quit();
		exit(2);
	}

	done = 0;
	bool OAM_TOGGLE = 0;
	
	//PPU ppu;
	//CPU_6502 cpu;
	

	NESCart *cart = new NESCart();

	

	
	
	

	//cpu.LoadFamicomFileToCPU(argv[1]);
	//cpu.WriteCodeBlocksForDisassembly();
	int ret = cart->Load(argv[1]);
	if(ret)
	{
		printf("Error loading %s...", argv[1]);
		switch(ret)
		{
		case FILE_OPEN_ERROR:
			printf("No such file\n");
			break;
		case FILE_READ_ERROR:
			printf("error reading file\n");
			break;
		case FILE_WRONG_FILETYPE:
			printf("incorrect file type\n");
			break;
		default:break;

		}
		return 0;
	}
	else
		printf("Successfully loaded %s\n", argv[1]);

//	printf("Number of 16kb rom banks: %d\nNumber of 8kb vrom banks: %d, mapper:0x%02X\n", ines.head.num_16kb_rom_banks, ines.head.num_8kb_vrom_banks, ines.mapper);

//	memcpy(cpu.memory,ines.CPU_MEMORY_MAP, 0x10000);

	M6502 cpu2(cart);
//	disassembler dis(ines.CPU_MEMORY_MAP);
//	dis.disassemble(0x8000);


//	cpu2.ppu = ::PPU();
	
//	cpu2.ppu.memory = ines.PPU_MEMORY_MAP;


	

//	cpu.Init();
//	cpu.StartExec();

//	SetSDLPalette(screen);

		
			


	cpu2.ppu_2.register_SDL(screen);


//	cpu2.ppu.doframe();

bool keep_key_one_frame = 0;
SDL_Event loop_event;
time_t t= time(NULL);
long frames = 0;
		while(!done)
		{
			

			//done = kbhit();
			
			//if(cpu2.isRunning() )
			{
		
	/*		if(cpu.ppu.nmi_set && !cpu.NMI_occured && cpu.RTI_occured && cpu.ppu.end_of_frame())
			{

			cpu.NMI();
			
			}
*/
/*				if(cpu.ppu.end_of_frame() && cpu.ppu.NMI_at_VBLANK) //we're now in vblank
				{
					cpu.NMI();
				}
*/

			//cpu.ppu.Update();
				/*if(cpu.cycles() >= 114 && !cpu.ppu.VBL_occured)
				{
					cpu.interruptTimer = 0;
					cpu.ppu.do_scanline();	
				}


				if(cpu.ppu.num_scanline>=240)//scanline 240...
				{
					if(cpu.ppu.NMI_at_VBLANK && !cpu.ppu.NMI_occured)
						cpu.NMI();
				}*/

		/*		while(cpu.cycles()<=113) //HBLANK
					cpu.Update();

				//if(cpu.cycles() >= 114)
				//{
					cpu.ppu.do_scanline();

					cpu.interruptTimer = 0;
				//}
				
				if(cpu.ppu.inVBlank())
				{
					
					if(cpu.ppu.NMIinVBlank() && !cpu.ppu.NMI_occured)
					{
						cpu.NMI();
					//	printf("NMI");
					}
					
				}

*/
				
				SDL_PollEvent(&loop_event);

				while(cpu2.NumCycles() <= 113 )
					cpu2.NextOpCode();
				cpu2.ppu_2.do_scanline();
				cpu2.ResetCycleCount();

				
			
				if(cpu2.ppu_2.inVBlank())
				{
							if(cpu2.ppu_2.NMI_in_VBlank() && !cpu2.ppu_2.NMI_occured)
					{
						cpu2.NMI();
									
					done = checkSDLEvent(loop_event, cpu2.memory);
			
						
						cpu2.ppu_2.BlitSDL(screen);

						SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
			
						frames++;
					//	printf("NMI");

						//update keyboard stuff
				
					}
				}




				if(time(NULL) - t > 1)
				{
					t = time(NULL);
					
					printf("fps: %03d", frames);
					printf("\b\b\b\b\b\b\b\b");
					frames = 0;
				}
			

			
			

			
			

			
			}
			
		}

	atexit(SDL_Quit);
	return 0;
}
