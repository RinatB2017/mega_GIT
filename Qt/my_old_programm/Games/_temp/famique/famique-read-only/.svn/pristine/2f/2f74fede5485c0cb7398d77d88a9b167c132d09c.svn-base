
#pragma once
#ifndef EMULATOR_H
#define EMULATOR_H
#include <QString>
#include <QTime>
#include <QTimer>
#include <QWaitCondition>
#include <vector>
#include <SDL/SDL.h>
#include "debugoutput.h"

#include "../emulator/famicom.h"
#include "../emulator/cpu/cpu2.h"
#include "../emulator/ppu/ppu.h"

#include "../emulator/cpu/tracer.h"
#include "../emulator/input.h"
#include "../emulator/config_file_parser.h"

#include "sdlframelimiter.h"



#include "nescart.h"
#include "ppu/ppu3.h"






typedef struct __BP
{
    word adress;
    bool break_at_nmi;
    bool break_at_brk;
    bool break_at_irq;

    byte break_on_opcode;

    word break_ident;


    bool is_active;
}BREAKPOINT;



class Emulator
{
public:
    Emulator();
    ~Emulator();
    void Update();

    int OpenROM(QString filename);
    void CloseROM();


    disassembler *GetDASMHandle() { return cpu->dis; }
    void PrintDebugString(QString str) { debug_output->Output(str); }

    void SetDebugOutputHandler(DebugOutput *d) { debug_output = d;}


    void set_controller(byte x){controller = x;}
    byte get_controller() { return controller;}

    NESInput *GetInputHandle() { return input; }
    CFGParser *GetConfig() { return config_file; }

    void SetSDLSurface(SDL_Surface *s) { scr = s; }
    bool isInitialized() { return init; }
    void Start();
    void Stop();
    bool isRunning();

    void UpdateKeyboard(MemoryManager *mem);
    void UpdateKeyboard(MemoryManager *mem, int key_pressed, bool pressed);

    //breakpoint funcs
    int AddBreakPoint(BREAKPOINT bp); //returns breakpoint ID
    void RemoveBreakPoint(int num_bp);    
    bool BP_occured() {bool ret = BP_has_triggered; if(BP_has_triggered == true)BP_has_triggered = false; return ret; }

    bool CheckBP();


    void ZAP();
    //misc cpu mumbo-jumbo
    CPU_SNAPSHOT StepCPU();
    M6502 *GetCPUHandle() { return cpu; }

    std::vector<BREAKPOINT> breakpoints; //uuuh, I know :-C. I couldn't be bothered to do linked lists, it's 06:23 AM for godssake!



    bool draw;
    bool audio_enabled;

    unsigned long *GetNameTableDebug(byte num);

    long frames_drawn;





    /* synchronization stuff used while rendering
       should really sync to the APU, but since it's not
       finished, whatevah :-o. it stays inaccurate in the meantime
*/

    bool fast_forward;
    void RegulateFrameRate(int fps);

    bool frame_limit;
    int current_fps;
    int nNextFrame;
    int nFramesSkipped;
    int nFramesSkippedBehind;
    int nMaxFrameSkip;

    QTime *tTicker;



 char *rompath;



private:
    iNesLoader *file_handle;
    NESCart *cart;
    M6502 *cpu;
    DebugOutput *debug_output;
    NESInput *input;
    CFGParser *config_file;
    SDLFrameLimiter flimit;



    SDL_Surface *scr;
    SDL_Event event;
    bool init;
    bool running;
    byte controller;
    void BPTrigger();
    bool BP_has_triggered;
    bool BP_enabled;


    void BlitScreen(bool draw);





};

#endif // EMULATOR_H
