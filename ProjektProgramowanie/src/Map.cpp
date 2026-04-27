#include "Map.h"

#include "object_init.h"

Map::Map() {
    map_width = 6400;
    map_height = 4800;
}

Map::~Map() {
    if (background_texture) SDL_DestroyTexture(background_texture);
    if (sciana_texture) SDL_DestroyTexture(sciana_texture);
    if (drzwi_texture) SDL_DestroyTexture(drzwi_texture);
}

void Map::dodaj_sciane(float x, float y) {

    lista_scian.push_back(Object(x, y, "ProjektProgramowanie/Prowizorycznetekstury/sciana.png"));
}

void Map::dodaj_drzwi(float x, float y) {
    lista_drzwi.push_back({Object(x, y, "ProjektProgramowanie/Prowizorycznetekstury/drzwi.png"),false});
}

bool Map::collision_objects_check()
{
    for(auto& sciana : lista_scian)
    {
        if(player.collision_check_player(sciana))
        return true;
    }
     for(auto& drzwi : lista_drzwi)
    {
        if(player.collision_check_player(drzwi.drzwi))
        return true;
    }
    return false;
}
void Map::map_render(SDL_Renderer* renderer, float camera_x, float camera_y, int window_w, int window_h) {

    float view_x = camera_x - (window_w / 2.0f);
    float view_y = camera_y - (window_h / 2.0f);

    if (view_x < 0) view_x = 0;
    if (view_y < 0) view_y = 0;
    if (view_x > map_width - window_w) view_x = map_width - window_w;
    if (view_y > map_height - window_h) view_y = map_height - window_h;

    SDL_FRect src_bg = { view_x, view_y, (float)window_w, (float)window_h };
    SDL_FRect dst_bg = { 0.0f, 0.0f, (float)window_w, (float)window_h };
    SDL_RenderTexture(renderer, map_obj.texture_get(), &src_bg, &dst_bg);

    for (auto& sciana : lista_scian) {
        SDL_FRect dst_sciana;
        dst_sciana.x = sciana.position_get().x - view_x;
        dst_sciana.y = sciana.position_get().y - view_y;
        dst_sciana.w = sciana.width_get();
        dst_sciana.h = sciana.height_get();

        if (dst_sciana.x + dst_sciana.w > 0 && dst_sciana.x < window_w &&
            dst_sciana.y + dst_sciana.h > 0 && dst_sciana.y < window_h) {
            SDL_RenderTexture(renderer, wall_obj.texture_get(), NULL, &dst_sciana);
        }
    }

    for (auto& drzwi : lista_drzwi) {
        SDL_FRect dst_drzwi = { drzwi.drzwi.position_get().x - view_x, drzwi.drzwi.position_get().y - view_y, drzwi.drzwi.width_get(), drzwi.drzwi.height_get() };
        if (dst_drzwi.x + dst_drzwi.w > 0 && dst_drzwi.x < window_w &&
            dst_drzwi.y + dst_drzwi.h > 0 && dst_drzwi.y < window_h) {

            if (!drzwi.otwarte) {
                SDL_RenderTexture(renderer, door_obj.texture_get(), NULL, &dst_drzwi);
            }
        }
    }
}
