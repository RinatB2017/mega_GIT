
#include <QMessageBox>
#include <SDL/SDL.h>


#include "debugoutput.h"
#include "ppu/palette.h"

#include "emulator.h"
Emulator::Emulator()
{

    this->cpu = NULL;

    this->file_handle = NULL;
    init = false;
    running = false;

    BP_has_triggered = false;
    BP_enabled = false;



    controller = 0;

    this->tTicker = new QTimer();
    tTicker->start();
    frames_drawn = 0;


    frame_limit = false;
    fast_forward = false;

    nMaxFrameSkip = 6;
    nFramesSkippedBehind = 5;
    nFramesSkipped = 0;
    nNextFrame = 0;

    audio_enabled = true;

    this->input = new NESInput();
    input->SetDefault();
    this->config_file = new CFGParser;
    if(config_file->Load() != FILE_OPEN_ERROR)
    {
        input->SetControllerBinding1(config_file->GetCFG()->Control1_keybindings,true);
        rompath = config_file->GetCFG()->ROM_PATH;
    }
    else
        rompath = 0;




}

Emulator::~Emulator()
{
    delete  tTicker;

    if(this->isInitialized())
        {
            delete cart;

            delete this->cpu;
        }

    this->config_file->UpdateConfigData(this->GetInputHandle()->GetController(0), this->GetInputHandle()->GetController(1), this->audio_enabled, rompath);
    this->config_file->Save();
delete input;
if(rompath)
delete [] rompath;

}

void Emulator::Start()
{
    if(init == false)
    {
        this->PrintDebugString("The emulator hasn't been initialized\n");
        return;
    }
    else
    {
        if(!cpu->isRunning())
        {
           cpu->Exec();
           if(this->BP_has_triggered)
              this->BP_has_triggered = false;
       }
           running = true;
    }


}

void Emulator::Stop()
{
    if(cpu->isRunning())
        cpu->Halt();
    running = false;



}


void Emulator::BPTrigger()
{
    //triggers BEFORE the instruction/whatever executes
    CPU_SNAPSHOT snap = cpu->GetCPUState();
    for(int i=0;i<breakpoints.size();i++)
    {
        if(breakpoints[i].is_active)
        {

            if(breakpoints[i].break_on_opcode == snap.next_opcode)
            {
                this->BP_has_triggered = true;
                return;

            }


            if((breakpoints[i].adress == snap.pc))
            {
               this->BP_has_triggered = true;
               return ;
            }
        }




    }

    BP_has_triggered = false;
    return ;



/*
    word adress;
    bool break_at_nmi;
    bool break_at_brk;
    bool break_at_irq;

    byte break_on_opcode;

    word break_ident;


    bool is_active;
  */

}
bool Emulator::CheckBP()
{
    if(BP_enabled)
    {

        BPTrigger();
    if(this->BP_has_triggered)
        {
           this->Stop();
           return true;
       }

     }
    return false;

}
#define OLD_SHAJT

void Emulator::Update()
{

//    cpu->memory->uAPU->cpu_cycles_total += NTSC_CYCLES_PER_HBLANK;//cpu->NumCycles();
//    cpu->memory->uAPU->cpu_cycles_remain += NTSC_CYCLES_PER_HBLANK; //cpu->NumCycles();
#ifndef OLD_SHAJT
    while(cpu->cycles < 29640)
    {
        cpu->NextOpCode();


        cpu->ppu_2.UGLY_HAX_PPU_CATCH_UP(cpu->GetCycleCount());


        if(cpu->ppu_2.inVBlank())
        {
       //     UpdateKeyboard(cpu->memory);

            if(cpu->ppu_2.NMI_in_VBlank() && !cpu->ppu_2.NMI_occured)
            {
                cpu->memory->control1.controller = controller;
                cpu->memory->control2.controller = controller;

                cpu->NMI();


            }


        }


        if(this->GetCPUHandle()->ppu_2.draw)
        {




/*
            if(fast_forward)
               this->RegulateFrameRate(200);

         //   if(this->frame_limit)
            if(this->frame_limit)
                this->RegulateFrameRate(0);
            else

*/
            this->BlitScreen(true);

          //  if(audio_enabled)
          //   cpu->memory->uAPU->RenderAudio(cpu->memory->uAPU->elapsed());

       //     flimit.Synchronize();
           cpu->ppu_2.draw = false;
        }



    }

    cpu->ResetCycleCount();


#endif
#ifdef OLD_SHAJT

    cpu->memory->uAPU->cpu_cycles_total += NTSC_CYCLES_PER_HBLANK;//cpu->NumCycles();
    cpu->memory->uAPU->cpu_cycles_remain += NTSC_CYCLES_PER_HBLANK; //cpu->NumCycles();

   // cpu->ppu_2->do_scanline();

    if(audio_enabled)
    {
        while(cpu->memory->uAPU->cpu_cycles_remain >= 0)
        {

            if(CheckBP())
                break;
            //In HSYNC - run CPU for the remainder of the HSYNC
            if(cpu->CPU_crash)
            {
                if(QMessageBox::warning(NULL, "FATAL", "The CPU has reached an unrecoverable state and will now die\nFatal PC:$"+
                                     QString::number(cpu->previous_state.pc, 16)+"\nCurrent PC:$"+
                                     QString::number(cpu->GetCPUState().pc, 16)+QString("\nWould you like to run the CPU for another cycle to see if the problem resolves itself? (emulator might crash)"),
                                     QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                    {
                    this->GetCPUHandle()->CPU_crash = false;
                }
                else
                {
                    cpu->PrintAllRegs();

                    Stop();
                PrintDebugString(QString("CPU crashed at PC=$")+QString::number(this->GetCPUHandle()->previous_state.pc, 16) + ". This might be due to a bad ROM dump or due to an emulator bug");
                break;
                }

            }
            cpu->NextOpCode();
            cpu->ppu_2->RunCycles(cpu->GetCycleCount() * 3); //synchronize CPU & PPU


           // cpu->ppu_2.UGLY_HAX_PPU_CATCH_UP(cpu->cycles);

            cpu->memory->uAPU->cpu_cycles_remain -= cpu->GetCycleCount();




        }

    }

    else
    {
        while((cpu->cycles) < NTSC_CYCLES_PER_HBLANK)
        {
            if(CheckBP())
                break;

            cpu->NextOpCode();
            if(cpu->CPU_crash)
            {
                QMessageBox::warning(NULL, "FATAL", "The CPU has reached an unrecoverable state and will now die");
                 Stop();
                break;
            }
        }
    }

            //we're not in HSYNC any more, draw scanline
     if(!audio_enabled)
         cpu->ResetCycleCount();





          //  cpu->ResetCycleCount();



        if(cpu->ppu_2->inVBlank())
        {
       //     UpdateKeyboard(cpu->memory);

            if(cpu->ppu_2->NMI_in_VBlank() && !cpu->ppu_2->NMI_occured)
            {
                cpu->memory->control1.controller = controller;
                cpu->memory->control2.controller = controller;

                cpu->NMI();


            }


        }


        if(this->GetCPUHandle()->ppu_2->draw)
        {




/*
            if(fast_forward)
               this->RegulateFrameRate(200);

         //   if(this->frame_limit)
            if(this->frame_limit)
                this->RegulateFrameRate(0);
            else

*/
            this->BlitScreen(true);

            if(audio_enabled)
             cpu->memory->uAPU->RenderAudio(cpu->memory->uAPU->elapsed());

       //     flimit.Synchronize();
           cpu->ppu_2->draw = false;
        }

        //one scanline is drawn, update timer




#endif




}



CPU_SNAPSHOT Emulator::StepCPU()
{
    //this function returns the last valid state from the CPU, and then proceeds to execute the next instruction

    CPU_SNAPSHOT previous_state = cpu->GetCPUState();

    if(BP_enabled)
    {

         BPTrigger();


    }
  
    cpu->NextOpCode();
    if(cpu->NumCycles() > 113)
    {
        //update ppu
        cpu->ppu.do_scanline();
        cpu->ResetCycleCount();
    }


    if(cpu->ppu.inVBlank())
    {
      //  UpdateKeyboard(cpu->memory);
        if(cpu->ppu.NMIinVBlank() && !cpu->ppu.NMI_occured)
        {
            cpu->NMI();
        }
    }



    return previous_state;

}

bool Emulator::isRunning()
{
    return running;

}


void Emulator::UpdateKeyboard(MemoryManager *mem, int key_pressed, bool pressed=true)
{
    switch(key_pressed)
    {

    case 0x01000012:
        if(pressed)
            SET_BIT(mem->control1.controller, 6);
        else
            CLR_BIT(mem->control1.controller, 6);

    break;
        case 0x01000014:
    if(pressed)

        SET_BIT(mem->control1.controller, 7);
    else
        CLR_BIT(mem->control1.controller, 7);

        break;
    case 0x01000004:
        if(pressed)



        SET_BIT(mem->control1.controller, 3);

        else
           CLR_BIT(mem->control1.controller, 3);


        break;


        case 0x01000013:
        if(pressed)
                SET_BIT(mem->control1.controller, 4);

        else
                CLR_BIT(mem->control1.controller, 4);
        break;

        case 0x01000015:
        if(pressed)
                SET_BIT(mem->control1.controller, 5);
        else
                CLR_BIT(mem->control1.controller, 5);

        break;

        case 0x41:
        if(pressed)
                SET_BIT(mem->control1.controller, 0);
        else
               CLR_BIT(mem->control1.controller, 0);


                break;
        case 0x42:
        if(pressed)
                SET_BIT(mem->control1.controller, 1);
        else
                CLR_BIT(mem->control1.controller, 1);


        case 0x20:
        if(pressed)
                SET_BIT(mem->control1.controller, 2);
        else
                CLR_BIT(mem->control1.controller, 2);


    }


}

void Emulator::UpdateKeyboard(MemoryManager *mem)
{
   SDL_PollEvent(&event);






                            if(event.type == SDL_KEYDOWN)
                                    {
                                     switch(event.key.keysym.sym)
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

                            else if(event.type == SDL_KEYUP)
                            {
                                             switch(event.key.keysym.sym)
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



}









/* for zeee debugger */


int Emulator::AddBreakPoint(BREAKPOINT bp)
{
    breakpoints.push_back(bp);
    BP_enabled = true;
    return breakpoints.size() -1;

}

void Emulator::RemoveBreakPoint(int num_bp)
{


  breakpoints.erase(breakpoints.begin() + num_bp);
  /*  breakpoints = 0;
    for(int i=0;i<num_bp - 1;i++)
        breakpoints = breakpoints->next;


    __BP *temp = breakpoints->next;
    if(temp->next == 0xFFFF) //next bp not allocated, just remove the entry we're on and set next to 0xFFFF
    {
        delete breakpoints->next;

*/
}



void Emulator::ZAP()
{

    this->Stop();
    breakpoints.erase(breakpoints.begin(), breakpoints.end());


    delete this->cpu;


}


unsigned long *Emulator::GetNameTableDebug(byte num)
{
    //get ptr to nametable
    byte t1, t2; //tile1 & 2
    byte color_high_bits;
    byte col;
    unsigned long *ret = new unsigned long [0xf000];
    byte *NES_PALETTE = SetupPalette();

    _nesRGBStruct *ghetto = ConvertToRGBStruct(NES_PALETTE);

    byte NES_PAL_ENTRY=0;

    byte tile_number = 0;

    byte tile_x = 0;

    word nt_adress;
    word mem_offset = 0;
    word draw_offset =0;

    byte *nt = this->GetCPUHandle()->ppu_2->GetNTPtr();// this->GetCPUHandle()->ppu_2.GetMemPtr(0x2000); //get the first name table

switch(num)
{
case 1:
    nt_adress = 0x000;
    break;
case 2:
    nt_adress = 0x400;
break;
case 3:
    nt_adress = 0x800;
break;
case 4:
    nt_adress = 0xc00;
break;
};

if(cart->mROMMapper->bVertical)
{
    if(nt_adress == 0x800)
        nt_adress = 0x000;
    if(nt_adress == 0xc00)
        nt_adress = 0x400;
}

if(cart->mROMMapper->bHorizontal)
{
    if(nt_adress == 0x800)
        nt_adress = 0x400;
    if(nt_adress = 0xc00)
        nt_adress = 0x0;
}





    for(int scanline=0;scanline<240;scanline++)
    {
        byte scanline_bgcol = GetCPUHandle()->ppu_2->GetPal(0);

        for(int i=0;i<256;i++)
        {//clear bg on current scanline
            ret[(scanline * 256)+i] = ghetto[scanline_bgcol].r << 16 | ghetto[scanline_bgcol].g << 8 | ghetto[scanline_bgcol].b;

        }
        for(int tx = 0; tx < 32; tx++)
        {

            tile_number = nt[nt_adress+((scanline/8)*32)+tx];
         //   mem_offset = tile_number * 16;
            mem_offset = ((GetCPUHandle()->ppu_2->DEBUG_EXPOSE_REGISTER(0x2000) & 0x10) ? 0x1000 : 0x0000) + (tile_number * 16);
            // mem_offset = (GET_BIT(GetCPUHandle()->ppu_2, 4)? 0x1000:0x0000) + tile_number * 16;


            color_high_bits = nt[nt_adress+0x3c0 + (((scanline/8)/4)*8) + (tx / 4)];//attribute tables resides here...
            t1 = this->cart->mROMMapper->ReadCHR(mem_offset+(scanline%8));
            t2 = this->cart->mROMMapper->ReadCHR(mem_offset+(scanline%8)+8);


            color_high_bits = (byte) (color_high_bits >> ((4*(((scanline/8)%4)/2))) + (2*((tx % 4)/2)));

            color_high_bits = (color_high_bits & 0x3) << 2;

            for(int i=0;i<8;i++)
            {
                col = color_high_bits + (((t2 &(1<<(7-i))) >> (7-i)) << 1) + ((t1 &(1<< (7-i))) >> (7-i));

                draw_offset = (scanline * 256 + (8*tx) +i);

               NES_PAL_ENTRY = this->GetCPUHandle()->ppu_2->GetPal(col&0x1f);


              if(col%4)
                   ret[draw_offset] =   ghetto[NES_PAL_ENTRY].r << 16 | ghetto[NES_PAL_ENTRY].g <<8 | ghetto[NES_PAL_ENTRY].b;

            /*


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



              */

            }




                              /*
                        tile1 = memory[tile_offset +(scroll_scanline%8)];

                        tile2 = memory[tile_offset +(scroll_scanline%8)+8];


                        //compute upper two bits of the palette from the attribute table of the current name table

                        palette_high = memory[name_table_base + 0x3c0 + (((scroll_scanline/8)/4) * 8) + (tile_x / 4)];

                    palette_high = (byte)(palette_high >> ((4 * (((scroll_scanline / 8) % 4) / 2)) + (2 * ((tile_x % 4) / 2))));

                        palette_high = (palette_high & 0x3) << 2;


*/



        }
    }
delete NES_PALETTE;
    return ret;

}


void Emulator::RegulateFrameRate(int fps)
{
    /* got this from Disch @ nesdev, cheers dude! */


    /*
     fps = desired frame rate, normally zero, but can be much higher if user is fast-forwarding
*/



    if(fps <= 0)
    {
        fps = 17; // NTSC uses 16.66667 ms per frame, rounding upwards to 17
    }

    else
        fps = 1000 / fps;







    signed long dif = (signed) (tTicker->elapsed() - nNextFrame);
    if(dif<0)
    {

        SDL_Delay(1);
        return;
    }

    nNextFrame += fps;

    if(dif >= (fps * nFramesSkippedBehind))
    {
        if(nFramesSkipped >= nMaxFrameSkip)
        {
            //resync
             tTicker->restart();
             nNextFrame = tTicker->elapsed();
            nFramesSkipped = 0;
        }
        else
        {
            ++nFramesSkipped;
            BlitScreen(false);
            //TODO: here we should update the PPU without blitting anything to the screen
            return ;
        }

    }
    nFramesSkipped = 0;

    BlitScreen(true);

}

void Emulator::BlitScreen(bool draw)
{




    if(!draw)
        return;



   //     SDL_LockSurface(cpu->ppu.bitmap);


    //    cpu->ppu.DEBUG_BLIT_TO_SDL();


       cpu->ppu_2->BlitSDL(this->scr);

        SDL_UpdateRect(this->scr, 0,0, scr->w, scr->h);

 //       SDL_UnlockSurface(cpu->ppu.bitmap);


   //     SDL_GL_SwapBuffers();



        frames_drawn++;


}


int Emulator::OpenROM(QString filename)
{

    if(init)
    {
        if(GetCPUHandle()->isRunning())
            Stop();
        init = false;
        this->PrintDebugString("\nResetting emulator state (inshallah)");

        delete cpu;
        cpu = 0;

        delete cart;
        cart = 0;

        this->PrintDebugString("No crash? so that went well :)\n");
    }
    cpu = 0;

    cart = 0;

#ifndef _NEW_OPENROM

    this->PrintDebugString("Loading "+filename);
    this->cart = new NESCart();
    QMessageBox err;
    switch(cart->Load(filename.toLatin1().data()))
    {
    case FILE_OPEN_ERROR:
        err.setText("Unable to open "+filename+"\n");
        PrintDebugString(err.text());
        err.exec();
        return FILE_OPEN_ERROR;
    break;
    case FILE_READ_ERROR:
        err.setText("Unable to read "+filename+"\n");
        PrintDebugString(err.text());
        err.exec();
        return FILE_READ_ERROR;
    break;
    case FILE_WRONG_FILETYPE:
        err.setText(filename+" is not a NES ROM or it has a corrupt/no header\n");
         PrintDebugString(err.text());
        err.exec();
        return FILE_WRONG_FILETYPE;
    break;

    case MAPPER_NOT_SUPPORTED:

       err.setText("the ROM contains an unsupported mapper type\nMapped is as an NROM (expect crashes)");
       PrintDebugString(err.text());
       err.exec();
       break;



    case 0:
        PrintDebugString("success!");
    break;


    default:break;
    }
    if((cart->mROM_Header.info2 & 0xC) == 0xC )
    {
        PrintDebugString("ROM header is iNES 2.0 - famique only supports the original iNES specification.");
    }
    if(cart->mROM_Header.num_8kb_ram_banks)
    {

        PrintDebugString("Onboard 8kb RAM banks: "+QString::number(cart->mROM_Header.num_8kb_ram_banks));
    }
    else
    {
        PrintDebugString("\nUses internal RAM\n");
    }

    if((cart->mROM_Header.video_mode & 1) == 1)
    {
        PrintDebugString("[!!UNSUPPORTED]Video mode: PAL");
    }
    else
        PrintDebugString("Video mode: NTSC\n");



    PrintDebugString("Battery Backed SRAM: "+QString(((cart->mROM_Header.info1 & 0x2) == 0x2)?"Enabled":"Disabled"));

    if((cart->mROM_Header.info1 & 0x2) == 0x2 || cart->bMapper == 1) //assume SRAM for all MMC1 games
    {
        if(cart->bMapper == 1)
            PrintDebugString("Temporary fulhack/Notice: This is a MMC1 ROM, so SRAM is enabled by default, since most ROM header are full of garbage\n");
        // see if we've got a SRAM-file already...
        QString sram_filename = filename + ".fsr";
        if(cart->mROMMapper->LoadSRAMFile(sram_filename.toLatin1().data()))
        {
            QMessageBox::warning(NULL, "Error", "An error occured while trying to access the SRAM file associated with this ROM");

        }
        else
            PrintDebugString("Load associated SRAM file: " + sram_filename);

    }

    PrintDebugString("PRG: "+QString::number((cart->bNum16kb*MEM_16k)/1024, 10)+"k" + " CHR: "+QString::number((cart->bNum8kb*MEM_8k)/1024,10) +"k");


    PrintDebugString("Mapper: "+QString::number( cart->bMapper, 10));

    if(cart->mROMMapper->bHorizontal)
        PrintDebugString("Horizontal mirroring");
    if(cart->mROMMapper->bVertical)
        PrintDebugString("Vertical mirroring");


    cpu = new M6502(cart);

    PrintDebugString("CPU lives @ "+QString::number((unsigned long)cpu, 16).toUpper());
    PrintDebugString("PPU lives @ "+QString::number((unsigned long)cpu->ppu_2, 16).toUpper());

    //    memcpy(cpu->ppu.memory, cart->mROMMapper->GetPPUMemMap(), MEM_8k);



        init=true;

      //  cpu->ppu.register_SDL(this->scr);



       this->Start();

#endif


#ifdef _OLD_OPENROM

    this->PrintDebugString("Attempting to load "+filename + "...");
    this->file_handle = new iNesLoader();
    QMessageBox err;
    switch(this->file_handle->Open(filename.toLatin1().data()))
    {
    case FILE_OPEN_ERROR:
        err.setText("Unable to open "+filename+"\n");
        PrintDebugString(err.text());
        err.exec();
        return FILE_OPEN_ERROR;
    break;
    case FILE_READ_ERROR:
        err.setText("Unable to read "+filename+"\n");
        PrintDebugString(err.text());
        err.exec();
        return FILE_READ_ERROR;
    break;
    case FILE_WRONG_FILETYPE:
        err.setText(filename+" is not a NES ROM or it has a corrupt header\n");
         PrintDebugString(err.text());
        err.exec();
        return FILE_WRONG_FILETYPE;
    break;
    default:break;


    }
     PrintDebugString("Success!\n");


     PrintDebugString("Loading "+QString::number((int)file_handle->head.num_16kb_rom_banks) + "x16kB ROM banks in to the CPU memory map\n");
    file_handle->Load16kbROM();
    if(file_handle->head.num_8kb_vrom_banks)
    {

    PrintDebugString("Loading "+QString::number((int)file_handle->head.num_8kb_vrom_banks)+"x8kB VROM banks in to the PPU memory map\n");

    file_handle->Load8kbVROM();
    }
    else
    {
        PrintDebugString("No CHRROM. zapping ppu memory map :o\n");
        memset(file_handle->PPU_MEMORY_MAP, 0, 0x3f20);
    }
    cpu = new M6502(file_handle->CPU_MEMORY_MAP);
    PrintDebugString("Initializing CPU @ 0x"+ QString::number((unsigned int)cpu, 16));

    memcpy(cpu->ppu.memory, file_handle->PPU_MEMORY_MAP, 0x3f20);

    cpu->ppu_2.setCHR(file_handle->PPU_MEMORY_MAP);

  //  PrintDebugString("Disassembling ROM");
    dis = new disassembler(cpu->memory->GetMemoryHandle());



    this->tTicker->restart();
    int size = dis->disassemble(file_handle->GetInitExec(), false);
   long  time = tTicker->elapsed();
   PrintDebugString(QString("Disassembled ROM - ")+QString::number(size,10)+" bytes :: "+QString::number(time,10)+"ms");

   PrintDebugString("video mode: "+ ((file_handle->head.video_mode >>7) == 1) ? "PAL":"NTSC");


    cpu->ppu.horizontal_mirroring = file_handle->horizontal_mirroring;
    cpu->ppu.vertical_mirroring = file_handle->vertical_mirroring;
    if(cpu->ppu.horizontal_mirroring == 1)
        PrintDebugString("Horizontal mirroring enabled");
    else if(cpu->ppu.vertical_mirroring)
        PrintDebugString("Vertical mirroring enabled");

    switch(file_handle->mapper)
    {
    case 0:
        PrintDebugString("Mapper: NROM\n");
        break;
    default:
        QMessageBox::warning(NULL,"fail :-c", "this ROM will probably not execute properly,\nsince it's using an unimplemented memory mapper" );
        PrintDebugString("Fatal: ["+QString::number(file_handle->mapper, 16)+"]::Mapper not supported, ROM file may not execute properly\n");
        break;
    }

    PrintDebugString("Execution vector at $"+QString::number((uint)cpu->GetCPUState().pc,16)+"\n");



    this->init = true;
    cpu->ppu.register_SDL(scr);

    PrintDebugString("Emulator initalized, you may now start execution");

    this->debug_output->update();





    delete file_handle;
#endif


    return 0;
}

