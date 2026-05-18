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
    lista_scian.push_back(Object(x, y, "ProjektProgramowanie/tex/sciana.png"));
}

void Map::dodaj_drzwi(float x, float y) {
    lista_drzwi.push_back({Object(x, y, "ProjektProgramowanie/tex/drzwi.png"),false});
}

bool Map::get_random_free_position(float width, float height, float& outX, float& outY, int maxAttempts, float margin)
{

    if (map_cols > 0 && map_rows > 0 && cell_size > 0.0f)
    {
        // Zwiększ marżynę do przynajmniej rozmiar obiektu
        float effectiveMargin = std::max(margin, width * 0.5f);

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
            // Sprawdzaj z marżyną - rozszerzony obszar
            int checkMargin = static_cast<int>(std::ceil(effectiveMargin / cell_size));
            for (int rr = std::max(0, r - checkMargin); rr < std::min(map_rows, r + needY + checkMargin) && ok; ++rr)
            {
                for (int cc = std::max(0, c - checkMargin); cc < std::min(map_cols, c + needX + checkMargin); ++cc)
                {
                    if (tile_map[rr][cc] != '.') { ok = false; break; }
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
            if (outX < effectiveMargin) outX = effectiveMargin;
            if (outY < effectiveMargin) outY = effectiveMargin;
            if (outX + width > map_width - effectiveMargin) outX = map_width - width - effectiveMargin;
            if (outY + height > map_height - effectiveMargin) outY = map_height - height - effectiveMargin;

            SDL_Log("Map: found free tile area at cell r=%d c=%d -> x=%f y=%f on attempt %d (need %d x %d)", r, c, outX, outY, attempt, needX, needY);
            return true;
        }

        SDL_Log("Map: failed to find free tile area after %d attempts", maxAttempts);
        return false;
    }

    // fallback: previous pixel-random method
    float effectiveMargin = std::max(margin, width * 0.5f);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(effectiveMargin, static_cast<float>(map_width) - width - effectiveMargin);
    std::uniform_real_distribution<float> distY(effectiveMargin, static_cast<float>(map_height) - height - effectiveMargin);

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

            if (x < sx + sw + effectiveMargin && x + width > sx - effectiveMargin && y < sy + sh + effectiveMargin && y + height > sy - effectiveMargin)
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

            if (x < sx + sw + effectiveMargin && x + width > sx - effectiveMargin && y < sy + sh + effectiveMargin && y + height > sy - effectiveMargin)
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
    float p_x = player.getX();
    float p_y = player.getY();
    float p_w = 100.0f;
    float p_h = 100.0f;
    for(auto& sciana : lista_scian)
    {
        if(player.collision_check_player(sciana))
        return true;
    }
     for(auto& drzwi : lista_drzwi)
    {
        if(!drzwi.otwarte) {
            Vec2f pos = drzwi.drzwi.position_get();
            if (p_x < pos.x + cell_size && p_x + p_w > pos.x && p_y < pos.y + cell_size && p_y + p_h > pos.y) {
                return true;
            }
        }
    }
    return false;
}
bool Map::collision_check_object(Object& obj)
{
    // Sprawdzenie kolizji obiektu ze ścianami i drzwiami
    // obj - obiekt do sprawdzenia (sprite z pozycją)

    Vec2f obj_pos = obj.position_get();
    float obj_w = obj.width_get();
    float obj_h = obj.height_get();

    // Sprawdzenie ze ścianami
    for(const auto& sciana : lista_scian)
    {
        Vec2f wall_pos = sciana.position_get();
        float wall_w = cell_size;
        float wall_h = cell_size;

        // AABB collision check (nie używaj radius)
        if (obj_pos.x < wall_pos.x + wall_w &&
            obj_pos.x + obj_w > wall_pos.x &&
            obj_pos.y < wall_pos.y + wall_h &&
            obj_pos.y + obj_h > wall_pos.y)
        {
            return true;
        }
    }

    // Sprawdzenie ze drzwiami
    for(const auto& drzwi : lista_drzwi)
    {
        if(!drzwi.otwarte) {
            Vec2f door_pos = drzwi.drzwi.position_get();
            float door_w = cell_size;
            float door_h = cell_size;

            if (obj_pos.x < door_pos.x + door_w &&
                obj_pos.x + obj_w > door_pos.x &&
                obj_pos.y < door_pos.y + door_h &&
                obj_pos.y + obj_h > door_pos.y)
            {
                return true;
            }
        }
    }

    return false;
}

const std::vector<Object>& Map::get_walls() const
{
    return lista_scian;
}

const std::vector<Drzwi>& Map::get_doors() const
{
    return lista_drzwi;
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
    SDL_RenderTexture(renderer, map_obj.texture_get(), NULL, &dst_bg);

    for (auto& sciana : lista_scian) {
        SDL_FRect dst_sciana;
        dst_sciana.x = sciana.position_get().x - camera.view_x_get();
        dst_sciana.y = sciana.position_get().y - camera.view_y_get();
        dst_sciana.w = cell_size;
        dst_sciana.h = cell_size;

        if (dst_sciana.x + dst_sciana.w > 0 && dst_sciana.x < window_w &&
            dst_sciana.y + dst_sciana.h > 0 && dst_sciana.y < window_h) {
            SDL_RenderTexture(renderer, wall_obj.texture_get(), NULL, &dst_sciana);
        }
    }

    for (auto& drzwi : lista_drzwi) {
        SDL_FRect dst_drzwi = { drzwi.drzwi.position_get().x - camera.view_x_get(), drzwi.drzwi.position_get().y - camera.view_y_get(), cell_size, cell_size };
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
void Map::interakcja_z_drzwiami(float gracz_x, float gracz_y, float gracz_w, float gracz_h) {
    float zasieg = 200.0f;
    float pole_x = gracz_x - zasieg;
    float pole_y = gracz_y - zasieg;
    float pole_w = gracz_w + (zasieg * 2);
    float pole_h = gracz_h + (zasieg * 2);

    for (auto& d : lista_drzwi) {
        Vec2f pos = d.drzwi.position_get();
        float door_x = pos.x;
        float door_y = pos.y;
        float door_w = cell_size;
        float door_h = cell_size;

        if (pole_x < door_x + door_w && pole_x + pole_w > door_x &&
            pole_y < door_y + door_h && pole_y + pole_h > door_y) {

            d.otwarte = !d.otwarte;
        }
    }
}
