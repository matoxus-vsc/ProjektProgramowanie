#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:

    Player(int health, int attack);

    int getHealth() const;
    int getAttack() const;

    void takeDamage(int amount);
    bool isAlive() const;
    void printStats() const;

private:
    int health;
    int attack;
    const char* name;
};

#endif
