#include "HUD.h"

#include "object_init.h"
#include <sstream>
#include <algorithm>
HUD::HUD()
{
    for(int i =0;i<PLAYERS;i++)
    {
        if(i==0)
            players.push_back(&player);
        else
            players.push_back(&bots[i-1]);
    }
    kill_feed_max_width = 0;
}

HUD::~HUD()
{
    //dtor
}

void HUD::fps_render(Uint64 &rend_frame)
{
    double fps = 1000.0 / static_cast<double>(rend_frame);
    std::stringstream fps_t;
    fps_t.str( "" );
    fps_t << "FPS:" << (int)fps;

    fps_text.text_load(fps_t.str().c_str(), t1.font_default_get(), t1.font_default_color_get());
}

void HUD::banner_bottom_render()
{
    SDL_FRect gradient
    {
        .x = 0,
        .y = static_cast<float>(0.95 * t1.window_height_get()),
        .w = static_cast<float>(t1.window_width_get()),
        .h = static_cast<float>(0.05 * t1.window_height_get())
    };

    std::stringstream hp_t;
    hp_t.str("");
    hp_t << "HP:" << player.getHealth() << " / " << player.max_health_get();

    hp_stat_text.text_load(hp_t.str().c_str(),t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());

    std::stringstream ammo_t;
    ammo_t.str("");
    ammo_t << "AMMO:" << player.ammo_in_mag_get() << " / " << player.spare_mags_get();

    ammo_stat_text.text_load(ammo_t.str().c_str(),t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());

    std::stringstream weapon_t;
    weapon_t.str("");
    weapon_t << "WEAPON:" << player.weapon_get();

    weapon_stat_text.text_load(weapon_t.str().c_str(),t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());

    SDL_SetRenderDrawColor(t1.renderer_get(), 0, 0, 0, 127);

    SDL_RenderRect(t1.renderer_get(), &gradient);
    SDL_RenderFillRect(t1.renderer_get(), &gradient);

    hp_stat_text.render(0.02 * t1.window_width_get(), 0.975 * t1.window_height_get() - hp_stat_text.height_get() * 0.5);

    ammo_stat_text.render(0.98 * t1.window_width_get() - ammo_stat_text.width_get(), 0.975 * t1.window_height_get() - ammo_stat_text.height_get() * 0.5);

    weapon_stat_text.render(0.5 * t1.window_width_get() - weapon_stat_text.width_get()* 0.5,  0.975 * t1.window_height_get() - weapon_stat_text.height_get() * 0.5);
}
void HUD::tab_render()
{
    SDL_FRect to_render
    {
        .x = 0.25 * t1.window_width_get(),
        .y = 0.25 * t1.window_height_get(),
        .w = 0.5 * t1.window_width_get(),
        .h = 0.5 * t1.window_height_get()
    };

    SDL_SetRenderDrawColor(t1.renderer_get(), 0, 0, 0, 127);
    SDL_RenderRect(t1.renderer_get(), &to_render);
    SDL_RenderFillRect(t1.renderer_get(), &to_render);

    tab_name_column_text.render(to_render.x + 0.25 * to_render.w, to_render.y);
    tab_kill_column_text.render(to_render.x + 0.62 * to_render.w, to_render.y);
    tab_death_column_text.render(to_render.x + 0.82 * to_render.w, to_render.y);

    SDL_SetRenderDrawColor(t1.renderer_get(), 255, 255, 255, 255);
    SDL_RenderLine(t1.renderer_get(), to_render.x + 0.77 * to_render.w, to_render.y, to_render.x + 0.77 * to_render.w, to_render.y + to_render.h);
    SDL_RenderLine(t1.renderer_get(), to_render.x + 0.57 * to_render.w, to_render.y, to_render.x + 0.57 * to_render.w, to_render.y + to_render.h);
    SDL_RenderLine(t1.renderer_get(), to_render.x, to_render.y + tab_name_column_text.height_get(), to_render.x + to_render.w, to_render.y + tab_name_column_text.height_get());



    for(int i=0; i<PLAYERS;i++)
    {
        tab_row_render(&to_render, i);
    }

}
void HUD::tab_row_render(SDL_FRect* tab, int index)
{
    std::stringstream k_t, d_t;

    k_t.str("");
    d_t.str("");

    k_t<<players[index]->kill_stat;
    d_t<<players[index]->death_stat;


    tab_name.text_load(players[index]->name.c_str(), t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());
    tab_kill.text_load(k_t.str().c_str(), t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());
    tab_death.text_load(d_t.str().c_str(), t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());

    tab_name.render(tab_name_column_text.position_get().x + 0.5 * tab_name_column_text.width_get() - 0.5 * tab_name.width_get(), tab_name_column_text.position_get().y + (index+1) * tab_name_column_text.height_get() + 0.01 * tab->h);
    tab_kill.render(tab_kill_column_text.position_get().x + 0.5 * tab_kill_column_text.width_get() - 0.5 * tab_kill.width_get(), tab_kill_column_text.position_get().y + (index+1) * tab_kill_column_text.height_get() + 0.01 * tab->h);
    tab_death.render(tab_death_column_text.position_get().x + 0.5 * tab_death_column_text.width_get() - 0.5 * tab_death.width_get(), tab_death_column_text.position_get().y + (index+1) * tab_death_column_text.height_get() + 0.01 * tab->h);



}
void HUD::tab_sort()
{

    std::sort(players.begin(), players.end(), [](const Player* ar, const Player* br)
              {
                  return ar->kill_stat > br->kill_stat;
              }
              );

}
void HUD::kill_feed_render()
{
    for(int i = 0;i<kill_feed.size();i++)
    {
        kill_feed_row_render(i);
    }
}
void HUD::kill_feed_push(int shooter, int dead)
{
    if(!kill_feed_disperse.start_check_get())
    {
        kill_feed_disperse.start();

    }
    std::stringstream to_push;
    to_push.str("");
    if(shooter==-1)
        to_push<<player.name;
    else
        to_push<<bots[shooter].name;
    to_push<<" > ";
    to_push<<bots[dead].name;

        kill_feed.push_back({to_push.str(), kill_feed_disperse.ticks_get()});

}
void HUD::kill_feed_row_render(int index)
{
    if(kill_feed[index].to_render!="")
    {
        if(kill_feed_disperse.ticks_get() - kill_feed[index].time_stamp<10000)
        {
            kill_feed_text.text_load(kill_feed[index].to_render.c_str(), t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());
        if(kill_feed_text.width_get()>kill_feed_max_width)
            kill_feed_max_width = kill_feed_text.width_get();
        SDL_FRect to_render
        {
            .x = t1.window_width_get() - 1.02 * kill_feed_max_width,
            .y = 1.02 * kill_feed_text.height_get() * index,
            .w = 1.02 * kill_feed_max_width,
            .h = 1.02 * kill_feed_text.height_get()
        };
        SDL_SetRenderDrawColor(t1.renderer_get(), 0, 0, 0, 127);
        SDL_RenderRect(t1.renderer_get(), &to_render);
        SDL_RenderFillRect(t1.renderer_get(), &to_render);

        kill_feed_text.render(to_render.x + 0.02 * kill_feed_text.width_get(), to_render.y + 0.02 * kill_feed_text.height_get());
        }
        else
        {
            kill_feed_disperse.ticks_set(kill_feed_disperse.ticks_get() - kill_feed[index].time_stamp);
            kill_feed.erase(kill_feed.begin() + index);
        }

    }
    else
    {
        kill_feed_disperse.finish();
    }


}
