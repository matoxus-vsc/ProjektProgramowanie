/** \author Igor Solny
 *
 * \file Controls.h
 *
 * \brief obsługa sterowania
 */


#ifndef CONTROLS_H
#define CONTROLS_H

#include "SDL3/SDL.h"

#include "structs.h"

/** \class Mouse
 *
 *  \brief ogólna klasa obsługi myszki
 */
class Mouse
{
    public:

        /** \brief Konstruktor - inicjacja właściwości
         *
         *
         */
        Mouse();

        ~Mouse();

        /** \brief ustawia pozycje myszki
         *
         * \param x float - współrzędna x
         * \param y float - współrzędna y
         * \return void
         *
         */
        void position_set(float x, float y);

        /** \brief obsługa eventów myszki
         *
         * \param e SDL_Event* - event myszki
         * \param s Game_state - aktualny stan gry
         * \return void
         *
         */
        void mouse_handler(SDL_Event* e, Game_state s); // celowanie sledzenie myszki

        /** \brief getter
         *
         * \return Vec2f - pozycja myszki
         *
         */
        Vec2f position_get();

        /** \brief DEBUG_MOUSE - wypisuje aktualna pozycję myszki
         *
         * \return void
         *
         */
        #ifdef DEBUG_MOUSE
            void position_print();
        #endif
    protected:

    private:
        Vec2f position;
};


#endif // CONTROLS_H
