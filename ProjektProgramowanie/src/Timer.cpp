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
    start_check = true;
    pause_check = false;
    pause_ticks = 0;
    start_ticks = SDL_GetTicks();
}
void Timer::pause()
{
    if(start_check && !pause_check)
    {
        pause_check = true;
        pause_ticks = SDL_GetTicks() - start_ticks;
        start_ticks = 0;

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

    start_check = false;
    pause_check = false;
    start_ticks = 0;
    pause_ticks = 0;
}
unsigned long int Timer::ticks_get()
{
    unsigned long int time = 0;
    if(start_check)
    {
        if(pause_check)
        {
           time = pause_ticks;
        }
        else
        {
            time = SDL_GetTicks() - start_ticks;
        }
    }
    return time;
}
bool Timer::pause_check_get()
{
    return pause_check;
}
bool Timer::start_check_get()
{
    return start_check;
}
