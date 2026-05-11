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

inline Technical t1;
inline Object door_obj, map_obj, wall_obj;
inline Camera camera;
inline Player player(100, 15);
inline Bot bot1(80, 10, 900.0f, 900.0f, 1200.0f, 900.0f);
inline Bot bot2(80, 10, 1400.0f, 1200.0f, 1700.0f, 1200.0f);
inline Mouse m1;
inline Timer fps_timer;
inline Text fps_text;
inline HUD h1;
inline Map arena;

#endif // OBJECT_INIT_H_INCLUDED
