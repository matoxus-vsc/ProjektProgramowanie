#include "Timer.h"

#include "../include/SDL3/SDL.h"

Timer::Timer()
{
    start_ticks = 0;
    pause_ticks = 0;
    start_check = false;
    pause_check = false;
}

Timer::~Timer()
{
    //dtor
}

void Timer::start()
{
    if(!start_check)
    {
        start_check = true;
        pause_check = false;
        pause_ticks = 0;
        start_ticks = SDL_GetTicks();
    }
}
void Timer::pause()
{
    if(start_check && !pause_check)
    {
        pause_check = true;
        pause_ticks = SDL_GetTicks();
    }
}
void Timer::unpause()
{
    if(start_check && pause_check)
    {
        pause_check = false;
        start_ticks = SDL_GetTicks() - pause_ticks;
        pause_ticks = 0;
    }
}
void Timer::finish()
{
    if(start_check)
    {
        start_check = false;
        pause_check = false;
        start_ticks = 0;
        pause_ticks = 0;
    }
}
