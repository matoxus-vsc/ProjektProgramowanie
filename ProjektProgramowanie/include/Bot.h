#ifndef BOT_H
#define BOT_H

#include "Player.h"

class Bot : public Player
{
public:
    Bot(int health, int attack);
    ~Bot();

    void init(float goalX, float goalY);
    void set_bot_index(int idx);  // Ustaw indeks bota dla spawn pointów
    void updateAI(Player& enemy);
    void render();
    void respawn();

    // Gettery dla kolizji pocisków
    bool is_shooting_get() const;
    float bullet_x_get() const;
    float bullet_y_get() const;
    void bullet_hit();
    int fire_cooldown_get() const;
    bool try_shoot_at(float targetCenterX, float targetCenterY, float distance);

    // Settery dla ataków bot-vs-bot
    void bullet_set_position(float x, float y);
    void bullet_set_velocity(float dx, float dy);
    void bullet_start_shooting();

private:
    float goal_x;
    float goal_y;
    unsigned int goal_seed;  // Unikalny seed dla losowych celów każdego bota
    int bot_index;  // Indeks bota - do wyboru spawn pointu

    float bot_speed;
    float bot_angle;

    bool is_shooting;
    float bullet_x;
    float bullet_y;
    float bullet_dx;
    float bullet_dy;
    float bullet_angle;
    float bullet_speed;

    int fire_cooldown;
    int hide_timer;
    int patrol_timer;
    int patrol_dir;
    int goal_change_timer;  // Timer do zmiany goal pointa

    float detection_range;

    // AI personality - każdy bot ma inną taktykę
    int bot_personality;  // 0 = aggressive, 1 = tactical, 2 = defensive
    int movement_timer;   // Timer dla taktycznego ruchu (strafe)
    float strafe_dir_x;   // Kierunek strafowania
    float strafe_dir_y;

    int burst_shots_left;
    int burst_pause_timer;

    void moveWithBounds(float dx, float dy);
    void change_goal();  // Zmień goal point na losowy
    void shoot_at(float targetCenterX, float targetCenterY);


};

#endif
