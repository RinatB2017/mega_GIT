#include "sdlframelimiter.h"

SDLFrameLimiter::SDLFrameLimiter()
{

}

void SDLFrameLimiter::init()
{
    this->frame_count = 0;
    this->rate = FPS_DEFAULT;
    this->tickrate = (1000.0f / (float) FPS_DEFAULT);
    this->last_tick = SDL_GetTicks();
}

void SDLFrameLimiter::setframerate(int targetrate)
{
    if((rate >= FPS_LOWER) && (rate <= FPS_UPPER))
    {
        this->frame_count = 0;
        this->rate = targetrate;
        this->tickrate = ((1000.0f) / (float)(rate));

    }
    else
        this->init();
}

Uint32 SDLFrameLimiter::getFramecount()
{
    return this->frame_count;
}

void SDLFrameLimiter::Synchronize()
{
    Uint32 current_ticks;
    Uint32 target;
    Uint32 delay;

    if(this->last_tick == 0)
        this->init();

    this->frame_count++;
    current_ticks =  SDL_GetTicks();
    target = this->last_tick + (Uint32) ((float)this->frame_count * this->rate);
    if(current_ticks <= target)
    {
        delay = target - current_ticks;
        SDL_Delay(delay);
    }
    else
    {
        this->frame_count = 0;
        this->last_tick = SDL_GetTicks();
    }
}
