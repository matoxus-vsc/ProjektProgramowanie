#include "Player.h"
#include <iostream>
#include <math.h>
#include "object_init.h"
Player::Player(int health, int attack)
    :health(health), attack(attack), x(100), y(150), angle(0.0), is_shooting(false), bullet_x(0), bullet_y(0), bullet_dx(0), bullet_dy(0), bullet_angle(0.0), bullet_speed(15.0f) {name = "Chuj";movement_speed=10.0;}
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
    bool move_key_press[4] = {false, false, false, false};
        if(key_board_state[SDL_SCANCODE_W] && y >0)
        {
            speed.y=-movement_speed;
            move_key_press[0] = true;
        }
        if(key_board_state[SDL_SCANCODE_S] && y + sprite.height_get()  <  arena.map_height)
        {
            speed.y=movement_speed;
            move_key_press[1] = true;
        }
        if(key_board_state[SDL_SCANCODE_D] && x + sprite.width_get() < arena.map_width)
        {
            speed.x=movement_speed;
            move_key_press[2] = true;
        }
        if(key_board_state[SDL_SCANCODE_A] && x>0)
        {
            speed.x=-movement_speed;
            move_key_press[3] = true;
        }
    x+=speed.x;
    if(arena.collision_objects_check())
    {
        x-=speed.x;
    }
    y+=speed.y;
    if(arena.collision_objects_check())
    {
        y-=speed.y;
    }

    speed.arg_set(0, 0);

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
bool Player::collision_check_player(Object& other)
{
    Vec2f player_centre(x + sprite.width_get() * 0.5f, y + sprite.width_get()*0.5f);
    float r = sprite.width_get() * 0.5f;

    float closest_x = player_centre.x;
    float closest_y = player_centre.y;

    if (player_centre.x < other.position_get().x)
        closest_x = other.position_get().x;
    else if (player_centre.x > other.position_get().x + other.width_get())
        closest_x = other.position_get().x + other.width_get();

    if (player_centre.y < other.position_get().y)
        closest_y = other.position_get().y;
    else if (player_centre.y > other.position_get().y + other.height_get())
        closest_y = other.position_get().y + other.height_get();

    float dx = player_centre.x - closest_x;
    float dy = player_centre.y - closest_y;

    return (dx * dx + dy * dy) <= (r * r);
}
float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}
