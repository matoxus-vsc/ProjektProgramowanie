/** \author Igor Solny
 *
 * \file object_init.h
 *
 * \brief Inicjujemy tu obiekty klas z plików: inline Nazwa_klasy nazwa_obiektu;
 */


#ifndef OBJECT_INIT_H_INCLUDED
#define OBJECT_INIT_H_INCLUDED

#include "Technical.h"
#include "Object.h"
#include "Player.h"
#include "Bot.h"
#include "Controls.h"
#include "Timer.h"
#include "HUD.h"
#include "Map.h"
#include "Camera.h"
#include <vector>

inline Technical t1;
inline Object door_obj, map_obj, wall_obj;
inline Camera camera;
inline Player player(100, 15);
inline std::vector<Bot> bots = { Bot(100,15), Bot(100,15), Bot(100,15), Bot(100,15) };
inline Mouse m1;
inline Timer fps_timer, kill_feed_disperse;
inline Text fps_text, hp_stat_text, ammo_stat_text, weapon_stat_text, tab_name_column_text, tab_kill_column_text, tab_death_column_text, tab_name, tab_kill, tab_death, kill_feed_text;
inline HUD h1;
inline Map arena;

#endif // OBJECT_INIT_H_INCLUDED
