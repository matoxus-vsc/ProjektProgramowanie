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
#include "Controls.h"

inline Technical t1;
inline Object player_obj, gun_obj, door_obj, map_obj, bullet_obj, wall_obj;
inline Player player(100, 15);
inline Mouse m1;


#endif // OBJECT_INIT_H_INCLUDED
