#include <iostream>
#include <sstream>
#include <math.h>
#include "../include/SDL3/SDL.h"
#include "../include/SDL3/SDL_main.h"

#include "object_init.h"
#include "Map.h"
#include "structs.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
        arena.wczytaj_z_pliku("Prowizorycznetekstury/uklad_mapy.txt", 78.0f);
        player.setPosition(150.0f, 150.0f);

        // init bots - każdy bot dostaje inny spawn point na bazie indeksu
        for (int i = 0; i < static_cast<int>(bots.size()); i++)
        {
            bots[i].set_bot_index(i);
            bots[i].init(2000.0f, 2000.0f);  // Init z celem, ale każdy respawa w innym miejscu
        }

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

                        // Player bullet -> bots
                        if (player.is_shooting_get())
                        {
                            for (auto &bot : bots)
                            {
                                float dx = player.bullet_x_get() - (bot.getX() + bot.sprite.width_get() * 0.5f);
                                float dy = player.bullet_y_get() - (bot.getY() + bot.sprite.height_get() * 0.5f);
                                float dist_sq = dx * dx + dy * dy;
                                float hit_radius = 30.0f;

                                if (dist_sq <= hit_radius * hit_radius)
                                {
                                    bot.takeDamage(player.getAttack());
                                    player.bullet_hit();
                                    SDL_Log("Player hit Bot! Health: %d", bot.getHealth());

                                    if (!bot.isAlive())
                                    {
                                        bot.respawn();
                                    }
                                    break;
                                }
                            }
                        }

                        // Bot bullets -> player
                        for (auto &bot : bots)
                        {
                            if (!bot.is_shooting_get())
                            {
                                continue;
                            }

                            float dx = bot.bullet_x_get() - (player.getX() + player.sprite.width_get() * 0.5f);
                            float dy = bot.bullet_y_get() - (player.getY() + player.sprite.height_get() * 0.5f);
                            float dist_sq = dx * dx + dy * dy;
                            float hit_radius = 30.0f;

                            if (dist_sq <= hit_radius * hit_radius)
                            {
                                player.takeDamage(bot.getAttack());
                                bot.bullet_hit();
                                SDL_Log("Bot hit Player! Health: %d", player.getHealth());
                                break;
                            }
                        }

                        // Bot-vs-Bot interaction: boty się zauważają i atakują
                        for(size_t i = 0; i < bots.size(); ++i)
                        {
                            Bot& bot1 = bots[i];
                            float bot1_centerX = bot1.getX() + bot1.sprite.width_get() * 0.5f;
                            float bot1_centerY = bot1.getY() + bot1.sprite.height_get() * 0.5f;
                            
                            for(size_t j = i + 1; j < bots.size(); ++j)
                            {
                                Bot& bot2 = bots[j];
                                float bot2_centerX = bot2.getX() + bot2.sprite.width_get() * 0.5f;
                                float bot2_centerY = bot2.getY() + bot2.sprite.height_get() * 0.5f;
                                
                                float dx = bot2_centerX - bot1_centerX;
                                float dy = bot2_centerY - bot1_centerY;
                                float distSq = dx * dx + dy * dy;
                                float dist = sqrtf(distSq);
                                
                                if (distSq <= 500.0f * 500.0f && distSq > 100.0f)
                                {
                                    bot1.try_shoot_at(bot2_centerX, bot2_centerY, dist);
                                }
                            }
                        }

                        // Sprawdzenie czy pociski botów trafiają inne boty
                        for(size_t i = 0; i < bots.size(); ++i)
                        {
                            Bot& shooter = bots[i];
                            
                            // Sprawdzenie czy pocisk bota trafia innych botów
                            for(size_t j = 0; j < bots.size(); ++j)
                            {
                                if (i == j) continue;  // Nie strzel do siebie
                                
                                Bot& target = bots[j];
                                
                                // Sprawdzenie czy pocisk trafia target
                                if (shooter.is_shooting_get())
                                {
                                    // Proximity check - czy pocisk jest blisko targetu
                                    float bullet_center_x = shooter.bullet_x_get();
                                    float bullet_center_y = shooter.bullet_y_get();
                                    
                                    float target_center_x = target.getX() + target.sprite.width_get() * 0.5f;
                                    float target_center_y = target.getY() + target.sprite.height_get() * 0.5f;
                                    
                                    float dx = bullet_center_x - target_center_x;
                                    float dy = bullet_center_y - target_center_y;
                                    float dist_sq = dx * dx + dy * dy;
                                    
                                    float hit_radius = 30.0f;
                                    
                                    if (dist_sq <= hit_radius * hit_radius)
                                    {
                                        target.takeDamage(10);
                                        shooter.bullet_hit();
                                        SDL_Log("Bot %d hit Bot %d! Health: %d", (int)i, (int)j, target.getHealth());
                                        
                                        if (!target.isAlive())
                                        {
                                            SDL_Log("Bot %d killed Bot %d!", (int)i, (int)j);
                                            target.respawn();
                                        }
                                    }
                                }
                            }
                        }

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

