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
        arena.wczytaj_z_pliku("ProjektProgramowanie/Prowizorycznetekstury/uklad_mapy.txt", 78.0f);
        player.setPosition(150.0f, 150.0f);
        tab_name_column_text.text_load("NAME", t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());
        tab_kill_column_text.text_load("KILLS", t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());
        tab_death_column_text.text_load("DEATHS", t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());



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
                            int index = 0;
                            for (auto &bot : bots)
                            {
                                float bullet_x = player.bullet_x_get();
                                float bullet_y = player.bullet_y_get();
                                float bot_x = bot.getX();
                                float bot_y = bot.getY();

                                float bullet_half_w = player.bullet.width_get() * 0.5f;
                                float bullet_half_h = player.bullet.height_get() * 0.5f;
                                float bot_half_w = bot.sprite.width_get() * 0.5f;
                                float bot_half_h = bot.sprite.height_get() * 0.5f;

                                float bullet_left = bullet_x - bullet_half_w;
                                float bullet_right = bullet_x + bullet_half_w;
                                float bullet_top = bullet_y - bullet_half_h;
                                float bullet_bottom = bullet_y + bullet_half_h;

                                float bot_left = bot_x + bot_half_w - bot_half_w;
                                float bot_right = bot_x + bot_half_w + bot_half_w;
                                float bot_top = bot_y + bot_half_h - bot_half_h;
                                float bot_bottom = bot_y + bot_half_h + bot_half_h;

                                if (!(bullet_right < bot_left || bullet_left > bot_right ||
                                      bullet_bottom < bot_top || bullet_top > bot_bottom))
                                {
                                    bot.takeDamage(player.getAttack());
                                    player.bullet_hit();
                                    SDL_Log("Player hit Bot! Health: %d", bot.getHealth());

                                    if (!bot.isAlive())
                                    {
                                        player.kill_stat++;
                                        bot.death_stat++;
                                        h1.tab_sort();
                                        h1.kill_feed_push(-1, index);

                                        bot.respawn();
                                    }
                                    break;
                                }
                                index++;
                            }
                        }

                        // Bot bullets -> player
                        for (auto &bot : bots)
                        {
                            if (!bot.is_shooting_get())
                            {
                                continue;
                            }

                            float bullet_x = bot.bullet_x_get();
                            float bullet_y = bot.bullet_y_get();
                            float player_x = player.getX();
                            float player_y = player.getY();

                            float bullet_half_w = bot.bullet.width_get() * 0.5f;
                            float bullet_half_h = bot.bullet.height_get() * 0.5f;
                            float player_half_w = player.sprite.width_get() * 0.5f;
                            float player_half_h = player.sprite.height_get() * 0.5f;

                            float bullet_left = bullet_x - bullet_half_w;
                            float bullet_right = bullet_x + bullet_half_w;
                            float bullet_top = bullet_y - bullet_half_h;
                            float bullet_bottom = bullet_y + bullet_half_h;

                            float player_left = player_x + player_half_w - player_half_w;
                            float player_right = player_x + player_half_w + player_half_w;
                            float player_top = player_y + player_half_h - player_half_h;
                            float player_bottom = player_y + player_half_h + player_half_h;

                            if (!(bullet_right < player_left || bullet_left > player_right ||
                                  bullet_bottom < player_top || bullet_top > player_bottom))
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
                                    float bullet_x = shooter.bullet_x_get();
                                    float bullet_y = shooter.bullet_y_get();
                                    float target_x = target.getX();
                                    float target_y = target.getY();

                                    float bullet_half_w = shooter.bullet.width_get() * 0.5f;
                                    float bullet_half_h = shooter.bullet.height_get() * 0.5f;
                                    float target_half_w = target.sprite.width_get() * 0.5f;
                                    float target_half_h = target.sprite.height_get() * 0.5f;

                                    float bullet_left = bullet_x - bullet_half_w;
                                    float bullet_right = bullet_x + bullet_half_w;
                                    float bullet_top = bullet_y - bullet_half_h;
                                    float bullet_bottom = bullet_y + bullet_half_h;

                                    float target_left = target_x + target_half_w - target_half_w;
                                    float target_right = target_x + target_half_w + target_half_w;
                                    float target_top = target_y + target_half_h - target_half_h;
                                    float target_bottom = target_y + target_half_h + target_half_h;

                                    if (!(bullet_right < target_left || bullet_left > target_right ||
                                          bullet_bottom < target_top || bullet_top > target_bottom))
                                    {
                                        target.takeDamage(10);
                                        shooter.bullet_hit();
                                        SDL_Log("Bot %d hit Bot %d! Health: %d", (int)i, (int)j, target.getHealth());

                                        if (!target.isAlive())
                                        {
                                            shooter.kill_stat++;
                                            target.death_stat++;
                                            h1.kill_feed_push(i, j);
                                            h1.tab_sort();

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

                        const bool* key_board_state = SDL_GetKeyboardState(NULL);
                        if(key_board_state[SDL_SCANCODE_TAB])
                        {
                            h1.tab_render();
                        }
                        h1.kill_feed_render();

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

