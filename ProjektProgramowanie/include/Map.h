#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>
#include <vector>

#include "Object.h"

/** \author Aleksander Poniński
 *
 * \file Map.h
 *
 * \brief Klasa mapy obsługująca tło, ściany, drzwi oraz kolizje.
 */


struct Drzwi {
    Object drzwi;
    bool otwarte;
};

class Map {
public:
    int map_width;
    int map_height;

    /** \brief Konstruktor ładujący tło, ściany i drzwi */
    Map();
    ~Map();

    void dodaj_sciane(float x, float y);

    void dodaj_drzwi(float x, float y);

    /** \brief sprawdza kolizje ze wszystkimi obiektami mapowymi
     *
     * \return bool
     *
     */
    bool collision_objects_check();

    void map_render(SDL_Renderer* renderer, float camera_x, float camera_y, int window_w, int window_h);

private:
    SDL_Texture* background_texture;
    SDL_Texture* sciana_texture;
    SDL_Texture* drzwi_texture;

    std::vector<Object> lista_scian;
    std::vector<Drzwi> lista_drzwi;
};
