#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <cstddef>
#include "Object.h"


class Player {
public:

    Player(int health, int attack);

    int getHealth() const;
    int getAttack() const;
    int max_health_get() const;

    const char* weapon_get();
    void takeDamage(int amount);
    void restoreHealth();
    bool isAlive() const;
    void printStats() const;

    void setPosition(float newX, float newY);
    void updatePosition(float targetX, float targetY);
    void render();
    bool is_shooting_get() const;
    float bullet_x_get() const;
    float bullet_y_get() const;
    std::size_t bullets_count_get() const;
    float bullet_x_at(std::size_t idx) const;
    float bullet_y_at(std::size_t idx) const;
    void bullet_remove_at(std::size_t idx);
    void bullet_hit();

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

    int kill_stat;
    int death_stat;
    std::string name;

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
    void respawn_reload();

protected:
    int health;
    int max_health;
    int attack;
    const char* weapon;

    float x;
    float y;
    float r;
    float movement_speed;
    Vec2f speed;
    double angle;

    bool is_shooting;

    struct ActiveBullet
    {
        float x;
        float y;
        float dx;
        float dy;
        float angle;
    };
    std::vector<ActiveBullet> active_bullets;

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
