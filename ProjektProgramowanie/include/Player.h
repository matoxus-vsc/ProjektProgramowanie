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

private:
    int health;
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
};

#endif
