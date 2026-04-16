#include <iostream>
#include "../include/SDL3/SDL.h"
#include "../include/SDL3/SDL_main.h"

#include "object_init.h"

#include "structs.h"

int main(int argc, char** argv)
{    
    if(!init(t1))
    {
        SDL_Log("Failed to initialize SDL, Error_technical code:%i %s",t1.err_code_get(), SDL_GetError());
        return 1;
    }
    else if(!media())
    {
        SDL_Log("Failed to initialize media %s", SDL_GetError());
        return 2;

    }
    else
    {
        bool exit = false;
        SDL_Event e;

        Game_state game_state = PLAYING;

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
                m1.mouse_handler(&e, game_state);
            }

                auto mousePos = m1.position_get();
                player.updatePosition(mousePos.x, mousePos.y); 

                SDL_SetRenderDrawColor(t1.renderer_get(), 255, 255, 255, 255);
                SDL_RenderClear(t1.renderer_get());

                map_obj.render(0, 0);

                player.render();

                SDL_RenderPresent(t1.renderer_get());


        }
        close(t1);
    }




    return 0;
}

