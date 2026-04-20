/** \author Igor Solny
 *
 * \file Timer.h
 * \
 * \brief timing w grze, potrzebny np. do fps
 *
 */


#ifndef TIMER_H
#define TIMER_H

#include "../include/SDL3/SDL.h"

/** \class Timer
 *
 *  \brief klasa stopera
 */
class Timer
{
    public:
        /** \brief Konstruktor - inicjacja właściwości
         *
         *
         */
        Timer();

        ~Timer();

        /** \brief start stopera
         *
         * \return void
         *
         */
        void start();

        /** \brief pauza stopera
         *
         * \return void
         *
         */
        void pause();

        /** \brief odpauzowanie stopera
         *
         * \return void
         *
         */
        void unpause();

        /** \brief reset stopera
         *
         * \return void
         *
         */
        void finish();

        /** \brief getter
         *
         * \return bool - czy wystartowano stoper
         *
         */

        Uint64 ticks_get();

        bool start_check_get();

        /** \brief getter
         *
         * \return bool - czy zapauzowano stoper
         *
         */
        bool pause_check_get();

    protected:

    private:
        Uint64 start_ticks;/**< naliczony czas na stoperze */
        Uint64 pause_ticks;/**< naliczony czas podczas pauzy */
        bool pause_check;/**< czy zapauzowano stoper */
        bool start_check;/**< czy wystartowano stoper */
};

#endif // TIMER_H
