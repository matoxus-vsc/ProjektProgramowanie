#include <iostream>
#include <sstream>
#include "../include/SDL3/SDL.h"
#include "../include/SDL3/SDL_main.h"

#include "object_init.h"
#include "Map.h"
#include "structs.h"

int main(int argc, char** argv)
{
    if(!init(t1))
    {
        SDL_Log("Failed to initialize SDL, Error_technical code:%i %s",t1.err_code_get(), SDL_GetError());
        return 1;
    }
    else if(!media(t1))
    {
        SDL_Log("Failed to initialize media %s", SDL_GetError());
        return 2;

    }
    else
    {
        Map arena(t1.renderer_get(), "ProjektProgramowanie/Prowizorycznetekstury/mapa.png", "ProjektProgramowanie/Prowizorycznetekstury/sciana.png", "ProjektProgramowanie/Prowizorycznetekstury/drzwi.png");
        arena.dodaj_sciane(400, 400, 200, 50);
        arena.dodaj_sciane(500, 400, 50, 400);
        arena.dodaj_drzwi(300, 350, 50, 100);
        bool exit = false;
        SDL_Event e;

        Game_state game_state = PLAYING;

        Uint64 rendered_frame = 0;
        double ms_per_frame = 1000.0 / t1.fps_target_get();

        while(!exit)
        {
            fps_timer.start();

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
                switch(game_state)
                {
                case PLAYING:
                    {
                        player.player_move_handler();

                        if(rendered_frame!= 0)
                        {
                            h1.fps_render(rendered_frame);
                        }


                        SDL_SetRenderDrawColor(t1.renderer_get(), 255, 255, 255, 255);
                        SDL_RenderClear(t1.renderer_get());

                        arena.map_render(t1.renderer_get(), player.getX(), player.getY(), 1280, 720);

                        fps_text.render(0, 0);

                        player.render();

                        SDL_RenderPresent(t1.renderer_get());

                        rendered_frame = fps_timer.ticks_get();

                        break;
                    }
                }



                if(rendered_frame < ms_per_frame)
                {
                    SDL_Delay(ms_per_frame - rendered_frame);
                    rendered_frame = fps_timer.ticks_get();
                }

        }
        close(t1);
    }




    return 0;
}

