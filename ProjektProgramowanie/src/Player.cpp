#include "Player.h"
#include <iostream>

Player::Player(int health, int attack)
    :health(health), attack(attack) {name = "Chuj";}
int Player::getHealth() const {
    return health;
}

int Player::getAttack() const {
    return attack;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::printStats() const {
    std::cout << "Player: " << name
              << " | HP: " << health
              << " | ATK: " << attack << std::endl;
}
