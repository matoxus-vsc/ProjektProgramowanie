#include "Player.h"
#include <iostream>
#include <math.h>

Player::Player(int health, int attack)
    :health(health), attack(attack), x(200), y(300), angle(0.0), is_shooting(false), bullet_x(0), bullet_y(0), bullet_dx(0), bullet_dy(0), bullet_angle(0.0), bullet_speed(15.0f) {name = "Chuj";movement_speed=10.0;}
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
    SDL_FPoint player_center = { static_cast<float>(sprite.width_get()) / 2.0f, static_cast<float>(sprite.height_get()) / 2.0f };
    sprite.render(x, y, angle, &player_center);

    SDL_FPoint gun_center = { -static_cast<float>(sprite.width_get()) / 2.0f, static_cast<float>(gun.height_get()) / 2.0f };
    gun.render(x + sprite.width_get(), y + sprite.height_get()/2.0f - gun.height_get()/2.0f, angle, &gun_center);

    if (is_shooting) {
        bullet_x += bullet_dx;
        bullet_y += bullet_dy;
        bullet.render(bullet_x, bullet_y, bullet_angle, nullptr);

        if (bullet_x < -2000 || bullet_x > 4000 || bullet_y < -2000 || bullet_y > 4000) {
            is_shooting = false;
        }
    } else {
        SDL_FPoint bullet_center = { -static_cast<float>(sprite.width_get()) / 2.0f - static_cast<float>(gun.width_get()), static_cast<float>(bullet.height_get()) / 2.0f };
        bullet.render(x + sprite.width_get() + gun.width_get(), y + sprite.height_get()/2.0f - bullet.height_get()/2.0f, angle, &bullet_center);
    }
}

void Player::player_move_handler()
{

    const bool* key_board_state = SDL_GetKeyboardState(NULL);
        if(key_board_state[SDL_SCANCODE_W])
        {
            y-=movement_speed;
        }
        if(key_board_state[SDL_SCANCODE_S])
        {
            y+=movement_speed;
        }
        if(key_board_state[SDL_SCANCODE_D])
        {
            x+=movement_speed;
        }
        if(key_board_state[SDL_SCANCODE_A])
        {
            x-=movement_speed;
        }

    float mouseX, mouseY;
    SDL_MouseButtonFlags mouseFlags = SDL_GetMouseState(&mouseX, &mouseY);

    float centerX = x + sprite.width_get() * 0.5f;
    float centerY = y + sprite.height_get() * 0.5f;
    angle = atan2(mouseY - centerY, mouseX - centerX) * 180.0 / M_PI;

    if ((mouseFlags & SDL_BUTTON_LMASK) && !is_shooting) {
        is_shooting = true;
        bullet_angle = angle;
        float rad = bullet_angle * M_PI / 180.0f;

        float offset = sprite.width_get() * 0.5f + gun.width_get();

        bullet_x = centerX + offset * cos(rad) - bullet.width_get() *0.5f;
        bullet_y = centerY + offset * sin(rad) - bullet.height_get() *0.5f;
        bullet_dx = cos(rad) * bullet_speed;
        bullet_dy = sin(rad) * bullet_speed;
    }


}
