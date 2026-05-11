#include "Bot.h"

#include <math.h>
#include <cstdlib>
#include <ctime>

#include "object_init.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Bot::Bot(int health, int attack) : Player(health, attack),
    goal_x(0), goal_y(0),
    bot_speed(4.0f), bot_angle(0.0), is_shooting(false),
    bullet_x(0.0f), bullet_y(0.0f), bullet_dx(0.0f), bullet_dy(0.0f), bullet_angle(0.0f), bullet_speed(11.0f),
    hide_timer(0), patrol_timer(0), patrol_dir(1), detection_range(480.0f)
{
    static bool seeded = false;
    if (!seeded) { std::srand(static_cast<unsigned int>(std::time(nullptr))); seeded = true; }
}

Bot::~Bot() {}

void Bot::init(float goalX, float goalY)
{
    goal_x = goalX; goal_y = goalY;
    respawn();
}

void Bot::respawn()
{
    restoreHealth();
    hide_timer = 0; patrol_timer = 0; patrol_dir = 1; is_shooting = false;
    bullet_x = bullet_y = bullet_dx = bullet_dy = 0.0f;

    float x,y;
    bool ok = arena.get_random_free_position(static_cast<float>(sprite.width_get()), static_cast<float>(sprite.height_get()), x, y, 5000, 4.0f);
    if(!ok)
    {
        // srodek mapy
        x = (arena.map_width - sprite.width_get()) * 0.5f;
        y = (arena.map_height - sprite.height_get()) * 0.5f;
    }

    SDL_Log("Bot: respawning at x=%f y=%f sprite_w=%d sprite_h=%d ammo=%d spare=%d", x, y, sprite.width_get(), sprite.height_get(), ammo_in_mag_get(), spare_mags_get());
    setPosition(x,y);
}

void Bot::moveWithBounds(float dx, float dy)
{
    float newX = getX() + dx;
    float newY = getY() + dy;

    float maxX = static_cast<float>(arena.map_width - sprite.width_get());
    float maxY = static_cast<float>(arena.map_height - sprite.height_get());

    if (newX < 0.0f) newX = 0.0f;
    if (newY < 0.0f) newY = 0.0f;
    if (newX > maxX) newX = maxX;
    if (newY > maxY) newY = maxY;

    setPosition(newX, newY);
}

void Bot::updateAI(Player& enemy)
{
    
    update_reload();

    if (!isAlive()) { respawn(); return; }

   
    if (ammo_in_mag_get() <= 0 && spare_mags_get() > 0 && !is_reloading())
    {
        start_reload();
    }

    float centerX = getX() + sprite.width_get() * 0.5f;
    float centerY = getY() + sprite.height_get() * 0.5f;

    float enemyCenterX = enemy.getX() + enemy.sprite.width_get() * 0.5f;
    float enemyCenterY = enemy.getY() + enemy.sprite.height_get() * 0.5f;

    float toEnemyX = enemyCenterX - centerX;
    float toEnemyY = enemyCenterY - centerY;
    float distSq = toEnemyX * toEnemyX + toEnemyY * toEnemyY;

    bool enemyVisible = distSq <= detection_range * detection_range;

    if (hide_timer > 0) hide_timer--;

    if (enemyVisible)
    {
        bot_angle = atan2(toEnemyY, toEnemyX) * 180.0 / M_PI;

        if (!is_shooting && fire_cooldown == 0 && !is_reloading())
        {
            if (ammo_in_mag_get() > 0)
            {
                is_shooting = true; bullet_angle = bot_angle;
                float rad = static_cast<float>(bullet_angle * M_PI / 180.0);
                float offset = sprite.width_get() * 0.5f + gun.width_get();
                bullet_x = centerX + offset * cos(rad) - bullet.width_get() * 0.5f;
                bullet_y = centerY + offset * sin(rad) - bullet.height_get() * 0.5f;
                bullet_dx = cos(rad) * bullet_speed; bullet_dy = sin(rad) * bullet_speed;
                hide_timer = 35;
            
                consume_one_ammo();
            }
            else
            {
                if (spare_mags_get() > 0) start_reload();
            }
        }

        if (hide_timer > 0 && distSq > 0.001f)
        {
            float len = sqrtf(distSq);
            moveWithBounds((-toEnemyX / len) * bot_speed, (-toEnemyY / len) * bot_speed);
        }
    }
    else
    {
        float toGoalX = goal_x - centerX; float toGoalY = goal_y - centerY; float goalDistSq = toGoalX*toGoalX + toGoalY*toGoalY;
        if (goalDistSq > 200.0f) { float len = sqrtf(goalDistSq); float dirX = toGoalX/len; float dirY = toGoalY/len; bot_angle = atan2(dirY, dirX) * 180.0 / M_PI; moveWithBounds(dirX * bot_speed, dirY * bot_speed); }
        else { patrol_timer++; if (patrol_timer > 20) { patrol_timer = 0; patrol_dir *= -1; } moveWithBounds(patrol_dir * bot_speed, 0.0f); bot_angle = (patrol_dir >= 0) ? 0.0 : 180.0; }
    }

    if (!is_shooting) return;

    bullet_x += bullet_dx; bullet_y += bullet_dy;
    if (bullet_x < 0 || bullet_x > arena.map_width || bullet_y < 0 || bullet_y > arena.map_height) { is_shooting = false; }
}

void Bot::render()
{
    float view_x = arena.camera_view_x_get(); float view_y = arena.camera_view_y_get();
    float screen_x = getX() - view_x; float screen_y = getY() - view_y;

    SDL_FPoint player_center = { static_cast<float>(sprite.width_get())/2.0f, static_cast<float>(sprite.height_get())/2.0f };
    sprite.render(screen_x, screen_y, bot_angle, &player_center);

    SDL_FPoint gun_center = { -static_cast<float>(sprite.width_get())/2.0f, static_cast<float>(gun.height_get())/2.0f };
    gun.render(screen_x + sprite.width_get(), screen_y + sprite.height_get()*0.5f - gun.height_get()*0.5f, bot_angle, &gun_center);

    if (is_shooting) { bullet.render(bullet_x - view_x, bullet_y - view_y, bullet_angle, nullptr); return; }

    SDL_FPoint bullet_center = { -static_cast<float>(sprite.width_get())*0.5f - static_cast<float>(gun.width_get()), static_cast<float>(bullet.height_get())*0.5f };
    bullet.render(screen_x + sprite.width_get() + gun.width_get(), screen_y + sprite.height_get()*0.5f - bullet.height_get()*0.5f, bot_angle, &bullet_center);
}
