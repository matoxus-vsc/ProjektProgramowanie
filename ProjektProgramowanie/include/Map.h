#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <iostream>
#include <vector>

#include "Object.h"
#include "Camera.h"

using namespace std;
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
    void wczytaj_z_pliku(const string& sciezka, float rozmiar_kratki);
    bool get_random_free_position(float width, float height, float& outX, float& outY, int maxAttempts = 100, float margin = 0.0f);

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

    /** \brief sprawdza kolizję obiektu z ścianami i drzwiami
     *
     * \param obj Object - obiekt do sprawdzenia
     * \return bool - czy jest kolizja
     *
     */
    bool collision_check_object(Object& obj);

    // Gettery dla botów aby sprawdzały kolizje
    const std::vector<Object>& get_walls() const;
    const std::vector<Drzwi>& get_doors() const;

    void camera_update(float camera_x, float camera_y, int window_w, int window_h);
    float camera_view_x_get() const;
    float camera_view_y_get() const;

    void map_render(SDL_Renderer* renderer, float camera_x, float camera_y, int window_w, int window_h);

private:
    SDL_Texture* background_texture;
    SDL_Texture* sciana_texture;
    SDL_Texture* drzwi_texture;

    Camera camera;

    int map_cols;
    int map_rows;
    float cell_size;
    std::vector<std::string> tile_map;

    std::vector<Object> lista_scian;
    std::vector<Drzwi> lista_drzwi;
};
