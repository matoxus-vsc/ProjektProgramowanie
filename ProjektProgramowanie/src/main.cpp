#include <iostream>
#include "../include/SDL3/SDL.h"
#include "../include/SDL3/SDL_main.h"

#include "object_init.h"
#include "Player.h"

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

                map_obj.render(0, 0);

                player_obj.render(200, 300);
                gun_obj.render(200 + player_obj.width_get(),300 + player_obj.height_get()/2 - gun_obj.height_get()/2);
                bullet_obj.render(200 + player_obj.width_get() + gun_obj.width_get(),300 + player_obj.height_get()/2 - bullet_obj.height_get()/2);

                player.printStats();
                player.takeDamage(30);
                player.printStats();

                SDL_RenderPresent(t1.renderer_get());


        }
        close(t1);
    }




    return 0;
}

