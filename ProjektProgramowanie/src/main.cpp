#include <iostream>
#include <sstream>
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
    else if(!media(t1))
    {
        SDL_Log("Failed to initialize media %s", SDL_GetError());
        return 2;

    }
    else
    {
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
                        player.player_move_handler(&e);

                        if(rendered_frame!= 0)
                        {
                            double fps = 1000.0 / static_cast<double>(rendered_frame);
                            std::stringstream fps_t;
                            fps_t.str( "" );
                            fps_t << "FPS:" << (int)fps;

                            fps_text.text_load(fps_t.str().c_str(), t1.font_default_get(), t1.font_default_color_get());
                        }


                        SDL_SetRenderDrawColor(t1.renderer_get(), 255, 255, 255, 255);
                        SDL_RenderClear(t1.renderer_get());

                        map_obj.render(0, 0);

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

