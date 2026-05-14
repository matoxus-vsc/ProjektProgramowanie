#include "Bot.h"

#include <math.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>
#include "object_init.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Bot::Bot(int health, int attack) : Player(health, attack),
    goal_x(0), goal_y(0),
    goal_seed(0),
    bot_index(-1),
    bot_speed(4.0f), bot_angle(0.0), is_shooting(false),
    bullet_x(0.0f), bullet_y(0.0f), bullet_dx(0.0f), bullet_dy(0.0f), bullet_angle(0.0f), bullet_speed(11.0f),
    fire_cooldown(0), hide_timer(0), patrol_timer(0), patrol_dir(1), goal_change_timer(0), detection_range(480.0f),
    movement_timer(0), strafe_dir_x(0.0f), strafe_dir_y(0.0f), burst_shots_left(0), burst_pause_timer(0)
{
    static bool seeded = false;
    static unsigned int bot_counter = 0;

    if (!seeded) { std::srand(static_cast<unsigned int>(std::time(nullptr))); seeded = true; }

    // Każdy bot dostaje unikalny seed na bazie licznika + losowość
    goal_seed = (bot_counter * 16807U) ^ static_cast<unsigned int>(std::time(nullptr));

    std::stringstream s;
    s.str("");
    s<<"Bot "<<bot_counter;
    name = s.str();
    bot_counter++;

    kill_stat = 0;
    death_stat = 0;

    // Przypisz losową osobowość botowi
    bot_personality = std::rand() % 3;  // 0 = aggressive, 1 = tactical, 2 = defensive
}

Bot::~Bot() {}

void Bot::set_bot_index(int idx)
{
    bot_index = idx;
}

void Bot::init(float goalX, float goalY)
{
    goal_x = goalX;
    goal_y = goalY;
    goal_change_timer = 0;
    respawn();
}

void Bot::respawn()
{
    restoreHealth();
    hide_timer = 0; patrol_timer = 0; patrol_dir = 1; is_shooting = false;
    fire_cooldown = 0;
    goal_change_timer = 0;
    burst_shots_left = 0;
    burst_pause_timer = 0;
    bullet_x = bullet_y = bullet_dx = bullet_dy = 0.0f;

    //  każdy bot ma swoj spawnpoint
    float safe_spawns[8][2] = {
        {800.0f, 500.0f},       // 0: Lewy górny
        {2400.0f, 500.0f},      // 1: Środek górny
        {4000.0f, 500.0f},      // 2: Prawy górny
        {4800.0f, 1500.0f},     // 3: Prawy środek górny
        {800.0f, 3600.0f},      // 4: Lewy dolny
        {2400.0f, 3600.0f},     // 5: Środek dolny
        {4000.0f, 3600.0f},     // 6: Prawy dolny
        {4800.0f, 2500.0f}      // 7: Prawy środek dolny
    };

    int num_spawns = 8;

    int spawn_idx;
    if (bot_index >= 0)
    {
        spawn_idx = (bot_index * 2) % num_spawns;
    }
    else
    {
        goal_seed = goal_seed * 1103515245U + 12345U;
        spawn_idx = (goal_seed / 65536U) % num_spawns;
    }

    int max_attempts = num_spawns;
    for (int attempt = 0; attempt < max_attempts; ++attempt)
    {
        int idx = (spawn_idx + attempt) % num_spawns;
        float x = safe_spawns[idx][0];
        float y = safe_spawns[idx][1];

        setPosition(x, y);
        // Kolizja z scianami Bota
        bool has_collision = false;
        for (const auto& sciana : arena.get_walls())
        {
            if (collision_check_player(const_cast<Object&>(sciana)))
            {
                has_collision = true;
                break;
            }
        }

        // Ze drzwiami
        if (!has_collision)
        {
            for (const auto& drzwi : arena.get_doors())
            {
                if (collision_check_player(const_cast<Object&>(drzwi.drzwi)))
                {
                    has_collision = true;
                    break;
                }
            }
        }

        // Jeśli bez kolizji
        if (!has_collision)
        {
            SDL_Log("Bot %d: respawned at safe_spawn[%d] x=%f y=%f", bot_index, idx, x, y);
            return;
        }
    }

    // Fallback - jeśli wszystkie spawn pointy mają kolizje
    float fallback_x = 800.0f;
    float fallback_y = 500.0f;
    setPosition(fallback_x, fallback_y);
    SDL_Log("Bot %d: respawned at fallback x=%f y=%f", bot_index, fallback_x, fallback_y);
}

void Bot::moveWithBounds(float dx, float dy)
{
    float oldX = getX();
    float oldY = getY();

    float newX = oldX + dx;
    float newY = oldY + dy;

    float maxX = static_cast<float>(arena.map_width - sprite.width_get());
    float maxY = static_cast<float>(arena.map_height - sprite.height_get());

    if (newX < 0.0f) newX = 0.0f;
    if (newY < 0.0f) newY = 0.0f;
    if (newX > maxX) newX = maxX;
    if (newY > maxY) newY = maxY;

    setPosition(newX, newY);

    // Sprawdzenie kolizji ze ścianami
    for (const auto& sciana : arena.get_walls())
    {
        if (collision_check_player(const_cast<Object&>(sciana)))
        {
            setPosition(oldX, oldY);
            return;
        }
    }

    // Sprawdzenie kolizji ze drzwiami
    for (const auto& drzwi : arena.get_doors())
    {
        if (collision_check_player(const_cast<Object&>(drzwi.drzwi)))
        {
            setPosition(oldX, oldY);
            return;
        }
    }
}

void Bot::shoot_at(float targetCenterX, float targetCenterY)
{
    float centerX = getX() + sprite.width_get() * 0.5f;
    float centerY = getY() + sprite.height_get() * 0.5f;
    float dx = targetCenterX - centerX;
    float dy = targetCenterY - centerY;
    float angle = atan2(dy, dx) * 180.0f / M_PI;
    float rad = static_cast<float>(angle * M_PI / 180.0f);

    bullet_angle = angle;
    float offset = sprite.width_get() * 0.5f + gun.width_get();
    bullet_x = centerX + offset * cos(rad) - bullet.width_get() * 0.5f;
    bullet_y = centerY + offset * sin(rad) - bullet.height_get() * 0.5f;
    bullet_dx = cos(rad) * bullet_speed;
    bullet_dy = sin(rad) * bullet_speed;
    is_shooting = true;
    consume_one_ammo();
    hide_timer = 35;
}

bool Bot::try_shoot_at(float targetCenterX, float targetCenterY, float distance)
{
    if (is_reloading())
    {
        return false;
    }

    if (ammo_in_mag_get() <= 0)
    {
        if (spare_mags_get() > 0)
        {
            start_reload();
        }
        return false;
    }

    if (distance <= 220.0f)
    {
        burst_shots_left = 0;
        burst_pause_timer = 0;

        if (fire_cooldown == 0)
        {
            shoot_at(targetCenterX, targetCenterY);
            fire_cooldown = 3;
            return true;
        }
        return false;
    }

    if (distance <= 500.0f)
    {
        if (burst_pause_timer > 0)
        {
            return false;
        }

        if (burst_shots_left == 0)
        {
            burst_shots_left = 3;
        }

        if (fire_cooldown == 0 && burst_shots_left > 0)
        {
            shoot_at(targetCenterX, targetCenterY);
            burst_shots_left--;
            fire_cooldown = 4;

            if (burst_shots_left == 0)
            {
                burst_pause_timer = 18;
            }
            return true;
        }

        return false;
    }

    burst_shots_left = 0;
    burst_pause_timer = 0;

    if (fire_cooldown == 0)
    {
        shoot_at(targetCenterX, targetCenterY);
        fire_cooldown = 24;
        return true;
    }

    return false;
}

void Bot::updateAI(Player& enemy)
{

    update_reload();

    // Zmniejsz cooldown
    if (fire_cooldown > 0) fire_cooldown--;
    if (burst_pause_timer > 0) burst_pause_timer--;
    if (movement_timer > 0) movement_timer--;

    //timer zmiany celu i zmień cel co 300 frames
    goal_change_timer++;
    if (goal_change_timer > 300)
    {
        change_goal();
    }

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
    float distance = sqrtf(distSq);

    bool enemyVisible = distSq <= detection_range * detection_range;

    if (hide_timer > 0) hide_timer--;

    if (enemyVisible)
    {
        // zauwazenie gracza
        bot_angle = atan2(toEnemyY, toEnemyX) * 180.0 / M_PI;

        // Ruch w zależności od osobowości
        if (bot_personality == 0)  // AGGRESSIVE - bezpośredni atak
        {
            // Atak
            if (distance > 300.0f)
            {
                float dirX = toEnemyX / distance;
                float dirY = toEnemyY / distance;
                moveWithBounds(dirX * bot_speed, dirY * bot_speed);
            }
        }
        else if (bot_personality == 1)  // TACTICAL - strafe i ruch
        {
            // Strafe lewo/prawo podczas strzału
            if (movement_timer == 0)
            {
                strafe_dir_x = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
                strafe_dir_y = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
                movement_timer = 20 + std::rand() % 20;  // Random duration
            }

            // Strafe
            float perpX = -toEnemyY / distance;
            float perpY = toEnemyX / distance;
            moveWithBounds(perpX * strafe_dir_x * bot_speed * 0.7f, perpY * strafe_dir_y * bot_speed * 0.7f);

            // Dystans
            if (distance < 250.0f)
            {
                moveWithBounds(-toEnemyX / distance * bot_speed, -toEnemyY / distance * bot_speed);
            }
        }
        else  // DEFENSIVE - uciekaj i strzelaj
        {
            // Uciekaj jeśli gracz jest blisko
            if (distance < 400.0f)
            {
                moveWithBounds(-toEnemyX / distance * bot_speed * 1.2f, -toEnemyY / distance * bot_speed * 1.2f);
            }
            else if (distance > 300.0f)
            {
                // Zbliż się jeśli gracz jest daleko
                moveWithBounds(toEnemyX / distance * bot_speed * 0.8f, toEnemyY / distance * bot_speed * 0.8f);
            }
        }

        try_shoot_at(enemyCenterX, enemyCenterY, distance);
    }
    else
    {
        // Gracz nie widoczny - patrol
        float toGoalX = goal_x - centerX;
        float toGoalY = goal_y - centerY;
        float goalDistSq = toGoalX*toGoalX + toGoalY*toGoalY;

        if (goalDistSq > 200.0f)
        {
            float len = sqrtf(goalDistSq);
            float dirX = toGoalX/len;
            float dirY = toGoalY/len;
            bot_angle = atan2(dirY, dirX) * 180.0 / M_PI;
            moveWithBounds(dirX * bot_speed, dirY * bot_speed);
        }
        else
        {
            // Patrol w celu
            patrol_timer++;
            if (patrol_timer > 20)
            {
                patrol_timer = 0;
                patrol_dir *= -1;
            }
            moveWithBounds(patrol_dir * bot_speed, 0.0f);
            bot_angle = (patrol_dir >= 0) ? 0.0 : 180.0;
        }
    }

    if (!is_shooting) return;

    bullet_x += bullet_dx;
    bullet_y += bullet_dy;
    if (bullet_x < -800 || bullet_x > arena.map_width + 800 || bullet_y < -800 || bullet_y > arena.map_height + 800)
    {
        is_shooting = false;
    }
}

void Bot::render()
{
    float view_x = arena.camera_view_x_get();
    float view_y = arena.camera_view_y_get();
    float screen_x = getX() - view_x;
    float screen_y = getY() - view_y;

    SDL_FPoint player_center = { static_cast<float>(sprite.width_get())/2.0f, static_cast<float>(sprite.height_get())/2.0f };
    sprite.render(screen_x, screen_y, bot_angle, &player_center);

    SDL_FPoint gun_center = { -static_cast<float>(sprite.width_get())/2.0f, static_cast<float>(gun.height_get())/2.0f };
    gun.render(screen_x + sprite.width_get(), screen_y + sprite.height_get()*0.5f - gun.height_get()*0.5f, bot_angle, &gun_center);

    if (is_shooting) {
        bullet.render(bullet_x - view_x, bullet_y - view_y, bullet_angle, nullptr);
        return;
    }

    SDL_FPoint bullet_center = { -static_cast<float>(sprite.width_get())*0.5f - static_cast<float>(gun.width_get()), static_cast<float>(bullet.height_get())*0.5f };
    bullet.render(screen_x + sprite.width_get() + gun.width_get(), screen_y + sprite.height_get()*0.5f - bullet.height_get()*0.5f, bot_angle, &bullet_center);
}

bool Bot::is_shooting_get() const
{
    return is_shooting;
}

float Bot::bullet_x_get() const
{
    return bullet_x;
}

float Bot::bullet_y_get() const
{
    return bullet_y;
}

void Bot::bullet_hit()
{
    is_shooting = false;
}

int Bot::fire_cooldown_get() const
{
    return fire_cooldown;
}

void Bot::bullet_set_position(float x, float y)
{
    bullet_x = x;
    bullet_y = y;
}

void Bot::bullet_set_velocity(float dx, float dy)
{
    bullet_dx = dx;
    bullet_dy = dy;
}

void Bot::bullet_start_shooting()
{
    is_shooting = true;
}

void Bot::change_goal()
{

    goal_seed = goal_seed * 1103515245U + 12345U;
    unsigned int rand1 = (goal_seed / 65536U) % 32768U;

    goal_seed = goal_seed * 1103515245U + 12345U;
    unsigned int rand2 = (goal_seed / 65536U) % 32768U;

    // Losowy punkt na mapie
    goal_x = 100.0f + (rand1 % static_cast<unsigned int>(arena.map_width - 200)) * 1.0f;
    goal_y = 100.0f + (rand2 % static_cast<unsigned int>(arena.map_height - 200)) * 1.0f;

    goal_change_timer = 0;
}
