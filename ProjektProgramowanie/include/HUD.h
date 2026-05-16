/** \author Igor Solny
 *
 * \file HUD.h
 *
 * \brief Nakładka na ekran podczas grania (info o hp np.)
 *
 */


#ifndef HUD_H
#define HUD_H

#define PLAYERS 5

#include "../include/SDL3/SDL.h"
#include "Player.h"

#include <string>
#include <vector>

/** \struct Kill_Feed
 *
 *  \brief przechowuje rząd kill feeda
 */
struct Kill_Feed
{
    std::string to_render;
    Uint64 time_stamp;
};
/** \class HUD
 *
 *  \brief klasa nakładki
 */
class HUD
{
    public:

        HUD();
        ~HUD();

        /** \brief renderowanie liczby fpsow
         *
         * \param rend_frame Uint64& - aktualna klatka
         * \return void
         *
         */
        void fps_render(Uint64 &rend_frame);

        /** \brief renderowanie panelu dolnego
         *
         * \return void
         *
         */
        void banner_bottom_render();

        /** \brief renderowanie tabu
         *
         * \return void
         *
         */
        void tab_render();

        /** \brief renderowanie rzędu tabu (statystyki danego gracza
         *
         * \param tab SDL_FRect* - tab
         * \param index int - który gracz
         * \return void
         *
         */
        void tab_row_render(SDL_FRect* tab, int index);

        /** \brief sortowanie tabeli po killach
         *
         * \return void
         *
         */
        void tab_sort();

        /** \brief renderowanie kill feedu
         *
         * \return void
         *
         */
        void kill_feed_render();
        /** \brief wprowadzenie rzędu do kill feeda
         *
         * \param shooter int - zabójca
         * \param dead int - ofiara
         * \return void
         *
         */
        void kill_feed_push(int shooter, int dead);
        /** \brief renderowanie pojedynczego rzędu kill feeda
         *
         * \param index int - który rząd
         * \return void
         *
         */
        void kill_feed_row_render(int index);

        std::vector<Kill_Feed> kill_feed; /**< przechowuje kill feed */

    protected:

    private:
        std::vector<Player*> players;/**< Wszyscy gracze */
        int kill_feed_max_width;/**< wyrównanie kill feeda */

};

#endif // HUD_H
