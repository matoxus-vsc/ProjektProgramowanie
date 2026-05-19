#include "Map.h"
#include <fstream>
#include <random>
#include <cmath>
#include <iostream>
#include "object_init.h"

using namespace std;

/**
 * \brief Konstruktor klasy Map
 */
Map::Map() {
    map_width = 6400;
    map_height = 4800;
    map_cols = 0;
    map_rows = 0;
    cell_size = 78.0f; // Standardowy rozmiar kafelka tekstury
}

/**
 * \brief Destruktor klasy Map
 */
Map::~Map() {
    if (background_texture) SDL_DestroyTexture(background_texture);
    if (sciana_texture) SDL_DestroyTexture(sciana_texture);
    if (drzwi_texture) SDL_DestroyTexture(drzwi_texture);
}

/**
 * \brief Tworzy nowy obiekt sciany w pamieci wektora.
 */
void Map::dodaj_sciane(float x, float y) {
    lista_scian.push_back(Object(x, y, "tex/sciana.png"));
}

/**
 * \brief Tworzy nowy obiekt drzwi w pamieci wektora.
 */
void Map::dodaj_drzwi(float x, float y) {
    lista_drzwi.push_back({Object(x, y, "ProjektProgramowanie/tex/drzwi.png"),false});
}

/**
 * \brief Wczytuje mape bazujac na znakach ASCII z pliku txt (# to sciana, D to drzwi).
 */
void Map::wczytaj_z_pliku(const string& sciezka, float rozmiar_kratki) {
    ifstream plik(sciezka);

    if (!plik.is_open()) {
        cerr << "Blad: Nie udalo sie otworzyc pliku mapy: " << sciezka << endl;
        return;
    }

    string linia;
    int wiersz = 0;
    tile_map.clear();

    while (getline(plik, linia)) {
        tile_map.push_back(linia);

        for (int kolumna = 0; kolumna < linia.length(); ++kolumna) {
            char znak = linia[kolumna];

            // Przeliczenie indeksu tablicy na realne piksele w grze
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
    map_cols = (map_rows > 0) ? static_cast<int>(tile_map[0].length()) : 0;
    cell_size = rozmiar_kratki;

    if (map_cols > 0 && map_rows > 0) {
        map_width = map_cols * static_cast<int>(cell_size);
        map_height = map_rows * static_cast<int>(cell_size);
    }
}

/**
 * \brief Szuka wolnego kafelka na zespawnowanie obiektu (np. gracza), unikajac nakladania sie ze scianami.
 */
bool Map::get_random_free_position(float width, float height, float& outX, float& outY, int maxAttempts, float margin)
{
    if (map_cols > 0 && map_rows > 0 && cell_size > 0.0f)
    {
        // Zwiekszenie marginesu bezpienrzenstwa wzgledem srodka obiektu
        float effectiveMargin = std::max(margin, width * 0.5f);

        int needX = static_cast<int>(std::ceil((double)width / (double)cell_size));
        int needY = static_cast<int>(std::ceil((double)height / (double)cell_size));

        if (needX > map_cols || needY > map_rows)
        {
            SDL_Log("Map: obiekt za duzy do siatki (need %d x %d cells, map %d x %d)", needX, needY, map_cols, map_rows);
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
            int checkMargin = static_cast<int>(std::ceil(effectiveMargin / cell_size));

            // Sprawdzanie okolicznych kratek
            for (int rr = std::max(0, r - checkMargin); rr < std::min(map_rows, r + needY + checkMargin) && ok; ++rr)
            {
                for (int cc = std::max(0, c - checkMargin); cc < std::min(map_cols, c + needX + checkMargin); ++cc)
                {
                    if (tile_map[rr][cc] != '.') { ok = false; break; }
                }
            }

            if (!ok) continue;

            // Obliczanie pikselowej pozycji na srodku wybranego wolnego kafelka
            float areaW = needX * cell_size;
            float areaH = needY * cell_size;
            float baseX = static_cast<float>(c) * cell_size;
            float baseY = static_cast<float>(r) * cell_size;

            outX = baseX + (areaW - width) * 0.5f;
            outY = baseY + (areaH - height) * 0.5f;

            // Utrzymanie obiektu w granicach okna
            if (outX < effectiveMargin) outX = effectiveMargin;
            if (outY < effectiveMargin) outY = effectiveMargin;
            if (outX + width > map_width - effectiveMargin) outX = map_width - width - effectiveMargin;
            if (outY + height > map_height - effectiveMargin) outY = map_height - height - effectiveMargin;

            SDL_Log("Map: znaleziono miejsce: r=%d c=%d -> x=%f y=%f (proby: %d)", r, c, outX, outY, attempt);
            return true;
        }

        SDL_Log("Map: nie znaleziono miejsca po %d probach", maxAttempts);
        return false;
    }

    // Metoda fallback - szukanie czysto pixelowe (gdy mapa nie ma poprawnej siatki)
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
            if (x < p.x + sc.width_get() + effectiveMargin && x + width > p.x - effectiveMargin && y < p.y + sc.height_get() + effectiveMargin && y + height > p.y - effectiveMargin)
            {
                ok = false; break;
            }
        }
        if (!ok) continue;

        // Sprawdzanie kolizji rzuconego punktu z drzwiami
        for (const auto& dr : lista_drzwi)
        {
            Vec2f p = dr.drzwi.position_get();
            if (x < p.x + dr.drzwi.width_get() + effectiveMargin && x + width > p.x - effectiveMargin && y < p.y + dr.drzwi.height_get() + effectiveMargin && y + height > p.y - effectiveMargin)
            {
                ok = false; break;
            }
        }
        if (!ok) continue;
        outX = x;
        outY = y;
        return true;
    }
    return false;
}

/**
 * \brief Twardy system kolizji gracza ze swiatem gry.
 */
bool Map::collision_objects_check()
{
    float p_x = player.getX();
    float p_y = player.getY();
    float p_w = 100.0f;
    float p_h = 100.0f;

    // 1. Sprawdzanie twardych scian mapy
    for(auto& sciana : lista_scian)
    {
        Vec2f pos = sciana.position_get();
        if (p_x < pos.x + cell_size && p_x + p_w > pos.x && p_y < pos.y + cell_size && p_y + p_h > pos.y) {
            return true; // Uderzenie w sciane
        }
    }
    for(auto& drzwi : lista_drzwi)
    {
        if(!drzwi.otwarte) { // Ignoruj otwarte drzwi
            Vec2f pos = drzwi.drzwi.position_get();
            if (p_x < pos.x + cell_size && p_x + p_w > pos.x && p_y < pos.y + cell_size && p_y + p_h > pos.y) {
                return true;
            }
        }
    }
    return false;
}

/**
 * \brief System kolizji dynamicznych obiektow (np. pociskow, promieni) ze swiatem gry.
 */
bool Map::collision_check_object(Object& obj)
{
    Vec2f obj_pos = obj.position_get();
    float obj_w = obj.width_get();
    float obj_h = obj.height_get();

    for(const auto& sciana : lista_scian)
    {
        Vec2f wall_pos = sciana.position_get();
        if (obj_pos.x < wall_pos.x + cell_size &&
            obj_pos.x + obj_w > wall_pos.x &&
            obj_pos.y < wall_pos.y + cell_size &&
            obj_pos.y + obj_h > wall_pos.y)
        {
            return true;
        }
    }
    for(const auto& drzwi : lista_drzwi)
    {
        if(!drzwi.otwarte) {
            Vec2f door_pos = drzwi.drzwi.position_get();
            if (obj_pos.x < door_pos.x + cell_size && obj_pos.x + obj_w > door_pos.x && obj_pos.y < door_pos.y + cell_size && obj_pos.y + obj_h > door_pos.y)
            {
                return true;
            }
        }
    }

    return false;
}

/**
 * \brief Obsluguje interakcje gracza z obiektami typu "drzwi".
 */
void Map::interakcja_z_drzwiami(float gracz_x, float gracz_y, float gracz_w, float gracz_h) {

    // Zdefiniowanie promienistego "zasiegu" w ktorym gracz moze uzywac drzwi
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

        // Sprawdzenie, czy konkretne drzwi znajduja sie w obszarze interakcji gracza
        if (pole_x < door_x + door_w && pole_x + pole_w > door_x && pole_y < door_y + door_h && pole_y + pole_h > door_y) {
            d.otwarte = !d.otwarte;
        }
    }
}

/** \brief Zwraca stala referencje do listy scian (Getter) */
const std::vector<Object>& Map::get_walls() const {
    return lista_scian;
}

/** \brief Zwraca stala referencje do listy drzwi (Getter) */
const std::vector<Drzwi>& Map::get_doors() const {
    return lista_drzwi;
}

/** \brief Przekazuje parametry do klasy kamery w celu przeliczenia offsetu renderowania */
void Map::camera_update(float camera_x, float camera_y, int window_w, int window_h) {
    camera.update(camera_x, camera_y, window_w, window_h, map_width, map_height);
}

/** \brief Pobiera zaktualizowana pozycje kamery na osi X */
float Map::camera_view_x_get() const {
    return camera.view_x_get();
}

/** \brief Pobiera zaktualizowana pozycje kamery na osi Y */
float Map::camera_view_y_get() const {
    return camera.view_y_get();
}

/**
 * \brief Wykonuje glowny algorytm rysowania swiata.
 * Uzywa techniki Frustum Culling - renderuje tylko obiekty znajdujace sie w obrebie ekranu monitora.
 */
void Map::map_render(SDL_Renderer* renderer, float camera_x, float camera_y, int window_w, int window_h) {

    camera_update(camera_x, camera_y, window_w, window_h);

    // Renderowanie statycznego tla mapy
    SDL_FRect src_bg = { camera.view_x_get(), camera.view_y_get(), (float)window_w, (float)window_h };
    SDL_FRect dst_bg = { 0.0f, 0.0f, (float)window_w, (float)window_h };
    SDL_RenderTexture(renderer, map_obj.texture_get(), NULL, &dst_bg);
    for (auto& sciana : lista_scian) {
        SDL_FRect dst_sciana;
        dst_sciana.x = sciana.position_get().x - camera.view_x_get();
        dst_sciana.y = sciana.position_get().y - camera.view_y_get();
        dst_sciana.w = cell_size;
        dst_sciana.h = cell_size;

        if (dst_sciana.x + dst_sciana.w > 0 && dst_sciana.x < window_w && dst_sciana.y + dst_sciana.h > 0 && dst_sciana.y < window_h) {
            SDL_RenderTexture(renderer, wall_obj.texture_get(), NULL, &dst_sciana);
        }
    }

    // Renderowanie zamknietych drzwi
    for (auto& drzwi : lista_drzwi) {
        SDL_FRect dst_drzwi = {
            drzwi.drzwi.position_get().x - camera.view_x_get(),
            drzwi.drzwi.position_get().y - camera.view_y_get(),
            cell_size, cell_size
        };
        if (dst_drzwi.x + dst_drzwi.w > 0 && dst_drzwi.x < window_w && dst_drzwi.y + dst_drzwi.h > 0 && dst_drzwi.y < window_h) {

            if (!drzwi.otwarte) {
                SDL_RenderTexture(renderer, door_obj.texture_get(), NULL, &dst_drzwi);
            }
        }
    }
}
