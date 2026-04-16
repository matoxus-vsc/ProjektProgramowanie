#include "Player.h"
#include <iostream>

Player::Player(int health, int attack)
    :health(health), attack(attack), x(200), y(300) {name = "Chuj";}
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

void Player::setPosition(float newX, float newY) { // Pozycja gracza
    x = newX;
    y = newY;
}

void Player::updatePosition(float targetX, float targetY) { // Podazanie za myszka
    float lerpFactor = 0.003f; // to zaleznosc jak szybko ma podazac, im mniej tym wolniej
    float destX = targetX - sprite.width_get() / 2.0f;
    float destY = targetY - sprite.height_get() / 2.0f;

    x += (destX - x) * lerpFactor;
    y += (destY - y) * lerpFactor;
}

void Player::render() { // Polaczenie gracza z bronia i pociskiem
    sprite.render(x, y);
    gun.render(x + sprite.width_get(), y + sprite.height_get()/2 - gun.height_get()/2);
    bullet.render(x + sprite.width_get() + gun.width_get(), y + sprite.height_get()/2 - bullet.height_get()/2);
}

