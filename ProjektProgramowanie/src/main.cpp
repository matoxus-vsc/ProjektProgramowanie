#include <iostream>
#include "../include/SDL3/SDL.h"
#include "../include/SDL3/SDL_main.h"

#include "object_init.h"

int main(int argc, char** argv)
{

    bool exit = false;
    SDL_Event e;

    while(!exit)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
            case SDL_EVENT_QUIT:
                {
                    exit = true;
                }
            }
        }
            SDL_SetRenderDrawColor(t1.renderer_get(), 255, 255, 255, 255);
            SDL_RenderClear(t1.renderer_get());

            SDL_SetRenderDrawColor(t1.renderer_get(), 255, 0, 0, 255);
            SDL_RenderLine(t1.renderer_get(), 200, 200, 300, 300);

            SDL_RenderPresent(t1.renderer_get());


    }

    close(t1);
    return 0;
}

