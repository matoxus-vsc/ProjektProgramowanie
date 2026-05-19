#include "Player.h"
#include <iostream>
#include <math.h>
#include "object_init.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Player::Player(int health, int attack)
    :health(health), max_health(health), attack(attack), x(100), y(150), angle(0.0), is_shooting(false)
{
    name = "Player";
    movement_speed=10.0;
    weapon = "AK-47";
    // ammo
    mag_capacity = 30;
    ammo_in_mag = mag_capacity;
    spare_mags = 2; // two spare mags
    reloading = false;
    reload_timer_frames = 0;

    // fire rate: frames between shots (lower = faster)
    fire_rate_frames = 6; // e.g., at 60fps -> 10 shots/sec
    fire_cooldown_frames = 0;
}

int Player::getHealth() const {
    return health;
}

int Player::getAttack() const {
    return attack;
}

int Player::max_health_get() const
{
    return max_health;
}

const char* Player::weapon_get()
{
    return weapon;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Player::restoreHealth()
{
    health = max_health;
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
    float view_x = arena.camera_view_x_get();
    float view_y = arena.camera_view_y_get();

    float screen_x = x - view_x;
    float screen_y = y - view_y;

    SDL_FPoint player_center = { static_cast<float>(sprite.width_get()) / 2.0f, static_cast<float>(sprite.height_get()) / 2.0f };
    sprite.render(screen_x, screen_y, angle - 90, &player_center);

    SDL_FPoint gun_center = { -static_cast<float>(sprite.width_get()) / 2.0f, static_cast<float>(gun.height_get()) / 2.0f };
    gun.render(screen_x + sprite.width_get(), screen_y + sprite.height_get()/2.0f - gun.height_get()/2.0f, angle, &gun_center);

    if (!active_bullets.empty()) {
        for (const auto& b : active_bullets)
        {
            bullet.render(b.x - view_x, b.y - view_y, b.angle, nullptr);
        }
    }
}

bool Player::is_shooting_get() const
{
    return !active_bullets.empty();
}

float Player::bullet_x_get() const
{
    if (active_bullets.empty()) return 0.0f;
    return active_bullets.front().x;
}

float Player::bullet_y_get() const
{
    if (active_bullets.empty()) return 0.0f;
    return active_bullets.front().y;
}

std::size_t Player::bullets_count_get() const
{
    return active_bullets.size();
}

float Player::bullet_x_at(std::size_t idx) const
{
    return active_bullets[idx].x;
}

float Player::bullet_y_at(std::size_t idx) const
{
    return active_bullets[idx].y;
}

void Player::bullet_remove_at(std::size_t idx)
{
    if (idx < active_bullets.size())
    {
        active_bullets.erase(active_bullets.begin() + static_cast<std::ptrdiff_t>(idx));
    }
}

void Player::bullet_hit()
{
    if (!active_bullets.empty())
    {
        active_bullets.erase(active_bullets.begin());
    }
}

void Player::player_move_handler()
{
    // handle reload timer
    update_reload();

    // reduce fire cooldown
    if (fire_cooldown_frames > 0) fire_cooldown_frames--;

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
        if(key_board_state[SDL_SCANCODE_W] && key_board_state[SDL_SCANCODE_S])
            speed.y = 0;
        if(key_board_state[SDL_SCANCODE_A] && key_board_state[SDL_SCANCODE_D])
            speed.x= 0;
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

    float mouseWorldX = mouseX + arena.camera_view_x_get();
    float mouseWorldY = mouseY + arena.camera_view_y_get();

    float centerX = x + sprite.width_get() * 0.5f;
    float centerY = y + sprite.height_get() * 0.5f;
    angle = atan2(mouseWorldY - centerY, mouseWorldX - centerX) * 180.0 / M_PI;

    // handle shoot/reload
    if (key_board_state[SDL_SCANCODE_R]) {
        if (!reloading && spare_mags > 0 && ammo_in_mag < mag_capacity) start_reload();
    }

    if ((mouseFlags & SDL_BUTTON_LMASK) && !reloading) {
        if (fire_cooldown_frames <= 0)
        {
            if (ammo_in_mag > 0) {
                consume_one_ammo();
                fire_cooldown_frames = fire_rate_frames;

                ActiveBullet new_bullet;
                new_bullet.angle = angle;
                float rad = static_cast<float>(new_bullet.angle * M_PI / 180.0f);
                float offset = sprite.width_get() * 0.5f + gun.width_get();
                float bullet_speed = 15.0f;
                new_bullet.x = centerX + offset * cos(rad) - bullet.width_get() *0.5f;
                new_bullet.y = centerY + offset * sin(rad) - bullet.height_get() *0.5f;
                new_bullet.dx = cos(rad) * bullet_speed;
                new_bullet.dy = sin(rad) * bullet_speed;

                // advance bullet immediately so it doesn't linger at muzzle
                new_bullet.x += new_bullet.dx;
                new_bullet.y += new_bullet.dy;

                active_bullets.push_back(new_bullet);
                is_shooting = true;
            } else {
                if (spare_mags > 0 && !reloading) start_reload();
            }
        }
    }

    // Update bullets positions
    for (std::size_t i = 0; i < active_bullets.size(); ) {
        active_bullets[i].x += active_bullets[i].dx;
        active_bullets[i].y += active_bullets[i].dy;

        bool bullet_out_of_bounds = (active_bullets[i].x < -800 || active_bullets[i].x > arena.map_width + 800 ||
                                      active_bullets[i].y < -800 || active_bullets[i].y > arena.map_height + 800);

        bool bullet_hit_wall = false;

        // Check collision with walls
        for (const auto& wall : arena.get_walls())
        {
            float bullet_half_w = bullet.width_get() * 0.5f;
            float bullet_half_h = bullet.height_get() * 0.5f;

            float bullet_left = active_bullets[i].x - bullet_half_w;
            float bullet_right = active_bullets[i].x + bullet_half_w;
            float bullet_top = active_bullets[i].y - bullet_half_h;
            float bullet_bottom = active_bullets[i].y + bullet_half_h;

            float wall_left = wall.position_get().x;
            float wall_right = wall.position_get().x + wall.width_get();
            float wall_top = wall.position_get().y;
            float wall_bottom = wall.position_get().y + wall.height_get();

            if (!(bullet_right < wall_left || bullet_left > wall_right ||
                  bullet_bottom < wall_top || bullet_top > wall_bottom))
            {
                bullet_hit_wall = true;
                break;
            }
        }

        // Check collision with doors (only if they are closed)
        if (!bullet_hit_wall)
        {
            for (const auto& drzwi : arena.get_doors())
            {
                // Only treat doors as obstacles if they are closed
                if (drzwi.otwarte) continue;

                float bullet_half_w = bullet.width_get() * 0.5f;
                float bullet_half_h = bullet.height_get() * 0.5f;

                float bullet_left = active_bullets[i].x - bullet_half_w;
                float bullet_right = active_bullets[i].x + bullet_half_w;
                float bullet_top = active_bullets[i].y - bullet_half_h;
                float bullet_bottom = active_bullets[i].y + bullet_half_h;

                float door_left = drzwi.drzwi.position_get().x;
                float door_right = drzwi.drzwi.position_get().x + drzwi.drzwi.width_get();
                float door_top = drzwi.drzwi.position_get().y;
                float door_bottom = drzwi.drzwi.position_get().y + drzwi.drzwi.height_get();

                if (!(bullet_right < door_left || bullet_left > door_right ||
                      bullet_bottom < door_top || bullet_top > door_bottom))
                {
                    bullet_hit_wall = true;
                    break;
                }
            }
        }

        if (bullet_out_of_bounds || bullet_hit_wall) {
            active_bullets.erase(active_bullets.begin() + static_cast<std::ptrdiff_t>(i));
        } else {
            ++i;
        }
    }

    is_shooting = !active_bullets.empty();
}

bool Player::collision_check_player(Object& other)
{
    Vec2f player_centre(x + sprite.width_get() * 0.5f, y + sprite.height_get()*0.5f);
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

// --- ammo i reload
void Player::start_reload()
{
    if (reloading) return;
    if (spare_mags <= 0) return;
    reloading = true;
    // reload time 4s
    reload_timer_frames = static_cast<int>(4.0f * static_cast<float>(t1.fps_target_get()));
    SDL_Log("Player: start reload, will take %d frames", reload_timer_frames);
}

void Player::update_reload()
{
    if (!reloading) return;
    if (reload_timer_frames > 0) reload_timer_frames--;
    if (reload_timer_frames <= 0)
    {
        reloading = false;
        if (spare_mags > 0)
        {
            spare_mags--;
            ammo_in_mag = mag_capacity;
            SDL_Log("Player: reload complete. ammo=%d spare_mags=%d", ammo_in_mag, spare_mags);
        }
    }
}

bool Player::is_reloading() const { return reloading; }
int Player::ammo_in_mag_get() const { return ammo_in_mag; }
int Player::spare_mags_get() const { return spare_mags; }
void Player::consume_one_ammo()
{
    if (ammo_in_mag > 0) ammo_in_mag--;
}
void Player::respawn_reload()
{
    ammo_in_mag = 30;
    spare_mags = 2;
}
