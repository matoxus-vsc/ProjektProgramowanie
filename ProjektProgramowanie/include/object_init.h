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
#include "Timer.h"

inline Technical t1;
inline Object door_obj, map_obj, wall_obj;
inline Player player(100, 15);
inline Mouse m1;
inline Timer fps;


#endif // OBJECT_INIT_H_INCLUDED
