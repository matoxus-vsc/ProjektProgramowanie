#include "Map.h"

Map::Map(SDL_Renderer* renderer, const std::string& map_path, const std::string& sciana_path, const std::string& drzwi_path) {
    map_width = 6400;
    map_height = 4800;

    background_texture = IMG_LoadTexture(renderer, map_path.c_str());
    sciana_texture = IMG_LoadTexture(renderer, sciana_path.c_str());
    drzwi_texture = IMG_LoadTexture(renderer, drzwi_path.c_str());

    if (!background_texture || !sciana_texture || !drzwi_texture) {
        std::cerr << "Blad wczytywania tekstur mapy: " << SDL_GetError() << std::endl;
    }
}

Map::~Map() {
    if (background_texture) SDL_DestroyTexture(background_texture);
    if (sciana_texture) SDL_DestroyTexture(sciana_texture);
    if (drzwi_texture) SDL_DestroyTexture(drzwi_texture);
}

void Map::dodaj_sciane(float x, float y, float w, float h) {
    lista_scian.push_back({x, y, w, h});
}

void Map::dodaj_drzwi(float x, float y, float w, float h) {
    lista_drzwi.push_back({x, y, w, h, false});
}

bool Map::sprawdz_kolizje(float obj_x, float obj_y, float obj_w, float obj_h) {
    for (const auto& sciana : lista_scian) {
        if (obj_x < sciana.x + sciana.w &&
            obj_x + obj_w > sciana.x &&
            obj_y < sciana.y + sciana.h &&
            obj_y + obj_h > sciana.y) {
            return true;
        }
    }
    return false;
}

void Map::map_render(SDL_Renderer* renderer, float camera_x, float camera_y, int window_w, int window_h) {
    if (!background_texture) return;

    float view_x = camera_x - (window_w / 2.0f);
    float view_y = camera_y - (window_h / 2.0f);

    if (view_x < 0) view_x = 0;
    if (view_y < 0) view_y = 0;
    if (view_x > map_width - window_w) view_x = map_width - window_w;
    if (view_y > map_height - window_h) view_y = map_height - window_h;

    SDL_FRect src_bg = { view_x, view_y, (float)window_w, (float)window_h };
    SDL_FRect dst_bg = { 0.0f, 0.0f, (float)window_w, (float)window_h };
    SDL_RenderTexture(renderer, background_texture, &src_bg, &dst_bg);

    for (const auto& sciana : lista_scian) {
        SDL_FRect dst_sciana;
        dst_sciana.x = sciana.x - view_x;
        dst_sciana.y = sciana.y - view_y;
        dst_sciana.w = sciana.w;
        dst_sciana.h = sciana.h;

        if (dst_sciana.x + dst_sciana.w > 0 && dst_sciana.x < window_w &&
            dst_sciana.y + dst_sciana.h > 0 && dst_sciana.y < window_h) {
            SDL_RenderTexture(renderer, sciana_texture, NULL, &dst_sciana);
        }
    }

    for (const auto& drzwi : lista_drzwi) {
        SDL_FRect dst_drzwi = { drzwi.x - view_x, drzwi.y - view_y, drzwi.w, drzwi.h };
        if (dst_drzwi.x + dst_drzwi.w > 0 && dst_drzwi.x < window_w &&
            dst_drzwi.y + dst_drzwi.h > 0 && dst_drzwi.y < window_h) {

            if (!drzwi.otwarte) {
                SDL_RenderTexture(renderer, drzwi_texture, NULL, &dst_drzwi);
            }
        }
    }
}
