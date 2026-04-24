#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>
#include <vector>

/** \author Aleksander Poniński
 *
 * \file Map.h
 *
 * \brief Klasa mapy obsługująca tło, ściany, drzwi oraz kolizje.
 */

struct Sciana {
    float x, y, w, h;
};

struct Drzwi {
    float x, y, w, h;
    bool otwarte;
};

class Map {
public:
    int map_width;
    int map_height;

    /** \brief Konstruktor ładujący tło, ściany i drzwi */
    Map(SDL_Renderer* renderer, const std::string& map_path, const std::string& sciana_path, const std::string& drzwi_path);
    ~Map();

    void dodaj_sciane(float x, float y, float w, float h);

    void dodaj_drzwi(float x, float y, float w, float h);

    bool sprawdz_kolizje(float obj_x, float obj_y, float obj_w, float obj_h);

    void map_render(SDL_Renderer* renderer, float camera_x, float camera_y, int window_w, int window_h);

private:
    SDL_Texture* background_texture;
    SDL_Texture* sciana_texture;
    SDL_Texture* drzwi_texture;

    std::vector<Sciana> lista_scian;
    std::vector<Drzwi> lista_drzwi;
};
