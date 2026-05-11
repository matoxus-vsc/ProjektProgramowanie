#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Object.h"


class Player {
public:

    Player(int health, int attack);

    int getHealth() const;
    int getAttack() const;

    void takeDamage(int amount);
    void restoreHealth();
    bool isAlive() const;
    void printStats() const;

    void setPosition(float newX, float newY);
    void updatePosition(float targetX, float targetY);
    void render();

    /** \brief ruch gracza po ekranie
     *
     * \return void
     *
     */
    void player_move_handler();

    /** \brief sprawdza kolizję ciała gracza z obiektem
     *
     * \param other Object&
     * \return bool
     *
     */
    bool collision_check_player(Object& other);

    Object sprite;
    Object gun;
    Object bullet;
    float getX() const;
    float getY() const;

    // --- ammo / reload  ---
    void start_reload();
    void update_reload();
    bool is_reloading() const;
    int ammo_in_mag_get() const;
    int spare_mags_get() const;
    void consume_one_ammo();

protected:
    int health;
    int max_health;
    int attack;
    const char* name;

    float x;
    float y;
    float r;
    float movement_speed;
    Vec2f speed;
    double angle;

    bool is_shooting;
    float bullet_x;
    float bullet_y;
    float bullet_dx;
    float bullet_dy;
    float bullet_angle;
    float bullet_speed;

    // ammo/reload
    int ammo_in_mag;        // ilosc akutalna
    int spare_mags;        // ilosc magazynkow pozostala
    int mag_capacity;      // ilosc naboi w magazynku
    bool reloading;
    int reload_timer_frames; // czas reload

  
    int fire_cooldown_frames; 
    int fire_rate_frames;

};

#endif
