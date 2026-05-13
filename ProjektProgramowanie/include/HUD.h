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

/** \class HUD
 *
 *  \brief klasa nakładki
 */
class HUD
{
    public:

        HUD();
        ~HUD();

        void fps_render(Uint64 &rend_frame);
        void banner_bottom_render();
        void tab_render();
        void tab_row_render(SDL_FRect* tab, int index);
        void tab_sort();



    protected:

    private:
        std::vector<Player*> players;
};

#endif // HUD_H
