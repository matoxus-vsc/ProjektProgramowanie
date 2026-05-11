#include "Map.h"
#include<fstream>
#include <random>
#include <cmath>
#include "object_init.h"
using namespace std;
Map::Map() {
    map_width = 6400;
    map_height = 4800;
    map_cols = 0;
    map_rows = 0;
    cell_size = 78.0f;
}

Map::~Map() {
    if (background_texture) SDL_DestroyTexture(background_texture);
    if (sciana_texture) SDL_DestroyTexture(sciana_texture);
    if (drzwi_texture) SDL_DestroyTexture(drzwi_texture);
}

void Map::dodaj_sciane(float x, float y) {
    lista_scian.push_back(Object(x, y, "Prowizorycznetekstury/sciana.png"));
}

void Map::dodaj_drzwi(float x, float y) {
    lista_drzwi.push_back({Object(x, y, "Prowizorycznetekstury/drzwi.png"),false});
}

bool Map::get_random_free_position(float width, float height, float& outX, float& outY, int maxAttempts, float margin)
{
    
    if (map_cols > 0 && map_rows > 0 && cell_size > 0.0f)
    {
        int needX = static_cast<int>(std::ceil((double)width / (double)cell_size));
        int needY = static_cast<int>(std::ceil((double)height / (double)cell_size));

        if (needX > map_cols || needY > map_rows)
        {
            SDL_Log("Map: object too large to fit in map grid (need %d x %d cells, map %d x %d)", needX, needY, map_cols, map_rows);
            return false;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distC(0, map_cols - needX);
        std::uniform_int_distribution<int> distR(0, map_rows - needY);

        for (int attempt = 0; attempt < maxAttempts; ++attempt)
        {
            int c = distC(gen);
            int r = distR(gen);

            bool ok = true;
            for (int rr = 0; rr < needY && ok; ++rr)
            {
                for (int cc = 0; cc < needX; ++cc)
                {
                    if (tile_map[r + rr][c + cc] != '.') { ok = false; break; }
                }
            }

            if (!ok) continue;

            // compute pixel position: place object centered in the found tile-rectangle
            float areaW = needX * cell_size;
            float areaH = needY * cell_size;
            float baseX = static_cast<float>(c) * cell_size;
            float baseY = static_cast<float>(r) * cell_size;

            outX = baseX + (areaW - width) * 0.5f;
            outY = baseY + (areaH - height) * 0.5f;

            // add small margin checks to be safe against neighbouring walls
            if (outX < 0) outX = 0;
            if (outY < 0) outY = 0;
            if (outX + width > map_width) outX = map_width - width;
            if (outY + height > map_height) outY = map_height - height;

            SDL_Log("Map: found free tile area at cell r=%d c=%d -> x=%f y=%f on attempt %d (need %d x %d)", r, c, outX, outY, attempt, needX, needY);
            return true;
        }

        SDL_Log("Map: failed to find free tile area after %d attempts", maxAttempts);
        return false;
    }

    // fallback: previous pixel-random method
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(margin, static_cast<float>(map_width) - width - margin);
    std::uniform_real_distribution<float> distY(margin, static_cast<float>(map_height) - height - margin);

    for (int i = 0; i < maxAttempts; ++i)
    {
        float x = distX(gen);
        float y = distY(gen);

        bool ok = true;
        for (const auto& sc : lista_scian)
        {
            Vec2f p = sc.position_get();
            float sx = p.x;
            float sy = p.y;
            float sw = static_cast<float>(sc.width_get());
            float sh = static_cast<float>(sc.height_get());

            if (x < sx + sw + margin && x + width > sx - margin && y < sy + sh + margin && y + height > sy - margin)
            {
                ok = false;
                break;
            }
        }

        if (!ok) continue;

        for (const auto& dr : lista_drzwi)
        {
            Vec2f p = dr.drzwi.position_get();
            float sx = p.x;
            float sy = p.y;
            float sw = static_cast<float>(dr.drzwi.width_get());
            float sh = static_cast<float>(dr.drzwi.height_get());

            if (x < sx + sw + margin && x + width > sx - margin && y < sy + sh + margin && y + height > sy - margin)
            {
                ok = false;
                break;
            }
        }

        if (!ok) continue;

        outX = x;
        outY = y;
        SDL_Log("Map: found free pos x=%f y=%f on attempt %d", outX, outY, i);
        return true;
    }

    SDL_Log("Map: failed to find free pos after %d attempts", maxAttempts);
    return false;
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

void Map::camera_update(float camera_x, float camera_y, int window_w, int window_h)
{
    camera.update(camera_x, camera_y, window_w, window_h, map_width, map_height);
}

float Map::camera_view_x_get() const
{
    return camera.view_x_get();
}

float Map::camera_view_y_get() const
{
    return camera.view_y_get();
}

void Map::map_render(SDL_Renderer* renderer, float camera_x, float camera_y, int window_w, int window_h) {

    camera_update(camera_x, camera_y, window_w, window_h);

    SDL_FRect src_bg = { camera.view_x_get(), camera.view_y_get(), (float)window_w, (float)window_h };
    SDL_FRect dst_bg = { 0.0f, 0.0f, (float)window_w, (float)window_h };
    SDL_RenderTexture(renderer, map_obj.texture_get(), &src_bg, &dst_bg);

    for (auto& sciana : lista_scian) {
        SDL_FRect dst_sciana;
        dst_sciana.x = sciana.position_get().x - camera.view_x_get();
        dst_sciana.y = sciana.position_get().y - camera.view_y_get();
        dst_sciana.w = sciana.width_get();
        dst_sciana.h = sciana.height_get();

        if (dst_sciana.x + dst_sciana.w > 0 && dst_sciana.x < window_w &&
            dst_sciana.y + dst_sciana.h > 0 && dst_sciana.y < window_h) {
            SDL_RenderTexture(renderer, wall_obj.texture_get(), NULL, &dst_sciana);
        }
    }

    for (auto& drzwi : lista_drzwi) {
        SDL_FRect dst_drzwi = { drzwi.drzwi.position_get().x - camera.view_x_get(), drzwi.drzwi.position_get().y - camera.view_y_get(), drzwi.drzwi.width_get(), drzwi.drzwi.height_get() };
        if (dst_drzwi.x + dst_drzwi.w > 0 && dst_drzwi.x < window_w &&
            dst_drzwi.y + dst_drzwi.h > 0 && dst_drzwi.y < window_h) {

            if (!drzwi.otwarte) {
                SDL_RenderTexture(renderer, door_obj.texture_get(), NULL, &dst_drzwi);
            }
        }
    }
}
void Map::wczytaj_z_pliku(const string& sciezka, float rozmiar_kratki) {
    ifstream plik(sciezka);

    if (!plik.is_open()) {
        cerr<<"Blad: Nie udalo sie otworzyc pliku mapy: "<<sciezka<<endl;
        return;
    }
    string linia;
    int wiersz = 0;
    tile_map.clear();
    while (getline(plik, linia)) {
        tile_map.push_back(linia);
        for (int kolumna = 0; kolumna < linia.length(); ++kolumna) {
            char znak = linia[kolumna];
            float fizyczny_x = kolumna * rozmiar_kratki;
            float fizyczny_y = wiersz * rozmiar_kratki;
            if (znak == '#') {
                dodaj_sciane(fizyczny_x, fizyczny_y);
            }
            else if (znak == 'D') {
                dodaj_drzwi(fizyczny_x, fizyczny_y);
            }
        }
        wiersz++;
    }
    plik.close();

    map_rows = static_cast<int>(tile_map.size());
    map_cols = (map_rows>0) ? static_cast<int>(tile_map[0].length()) : 0;
    cell_size = rozmiar_kratki;
    if (map_cols>0 && map_rows>0) {
        map_width = map_cols * static_cast<int>(cell_size);
        map_height = map_rows * static_cast<int>(cell_size);
    }
}
