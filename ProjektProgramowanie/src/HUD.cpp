#include "HUD.h"

#include "object_init.h"
#include <sstream>
HUD::HUD()
{
    //ctor
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
        .y = 0.95 * t1.window_height_get(),
        .w = t1.window_width_get(),
        .h = 0.05 * t1.window_height_get()
    };

    std::stringstream hp_t;
    hp_t.str("");
    hp_t<<"HP:"<<player.getHealth();

    hp_stat_text.text_load(hp_t.str().c_str(),t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());

    std::stringstream ammo_t;
    ammo_t.str("");
    ammo_t<<"AMMO:"<<player.current_ammo_get()<<" / "<<player.reloads_get();

    ammo_stat_text.text_load(ammo_t.str().c_str(),t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());

    std::stringstream weapon_t;
    weapon_t.str("");
    weapon_t<<"WEAPON:"<<player.weapon_get();

    weapon_stat_text.text_load(weapon_t.str().c_str(),t1.font_banner_bottom_get(), t1.font_banner_bottom_color_get());

    SDL_SetRenderDrawColor(t1.renderer_get(), 0, 0, 0, 127);

    SDL_RenderRect(t1.renderer_get(), &gradient);
    SDL_RenderFillRect(t1.renderer_get(), &gradient);

    hp_stat_text.render(0.02 * t1.window_width_get(), 0.975 * t1.window_height_get() - hp_stat_text.height_get() * 0.5);

    ammo_stat_text.render(0.98 * t1.window_width_get() - ammo_stat_text.width_get(), 0.975 * t1.window_height_get() - ammo_stat_text.height_get() * 0.5);

    weapon_stat_text.render(0.5 * t1.window_width_get() - weapon_stat_text.width_get()* 0.5,  0.975 * t1.window_height_get() - weapon_stat_text.height_get() * 0.5);
}
