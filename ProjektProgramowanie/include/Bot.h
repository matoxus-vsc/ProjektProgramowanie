#ifndef BOT_H
#define BOT_H

#include "Player.h"

class Bot : public Player
{
public:
    Bot(int health, int attack);
    ~Bot();

    void init(float goalX, float goalY);
    void updateAI(Player& enemy);
    void render();
    void respawn();

private:
    float goal_x;
    float goal_y;

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

    float detection_range;

    void moveWithBounds(float dx, float dy);
};

#endif
