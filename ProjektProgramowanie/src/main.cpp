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
        arena.wczytaj_z_pliku("ProjektProgramowanie/Prowizorycznetekstury/uklad_mapy.txt", 78.0f);
        player.setPosition(150.0f, 150.0f);

        // init bots
        if(bots.size() > 0) bots[0].init(1200.0f,900.0f);
        if(bots.size() > 1) bots[1].init(1700.0f,1200.0f);
        if(bots.size() > 2) bots[2].init(4000.0f,3000.0f);
        if(bots.size() > 3) bots[3].init(5000.0f,3500.0f);

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
                        for(auto &b : bots) b.updateAI(player);

                        if(rendered_frame!= 0)
                        {
                            h1.fps_render(rendered_frame);
                        }


                        SDL_SetRenderDrawColor(t1.renderer_get(), 255, 255, 255, 255);
                        SDL_RenderClear(t1.renderer_get());


                        int win_w = t1.window_width_get();
                        int win_h = t1.window_height_get();


                        float playerCenterX = player.getX() + player.sprite.width_get() * 0.5f;
                        float playerCenterY = player.getY() + player.sprite.height_get() * 0.5f;
                        arena.map_render(t1.renderer_get(), playerCenterX, playerCenterY, win_w, win_h);

                        fps_text.render(0, 0);

                        for(auto &b : bots) b.render();
                        player.render();
                        h1.banner_bottom_render();

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

