#ifndef SDLFRAMELIMITER_H
#define SDLFRAMELIMITER_H
#include <SDL.h>
#define FPS_UPPER 200
#define FPS_LOWER 1
#define FPS_DEFAULT 60//NTSC framerate.

class SDLFrameLimiter
{
public:
    SDLFrameLimiter();
    void init();
    void setframerate(int rate);
    Uint32 getFramecount();
    void Synchronize();

private:
    Uint32 frame_count;
    float tickrate;
    Uint32 last_tick;
    Uint32 rate;

};

#endif // SDLFRAMELIMITER_H
