/** \author Igor Solny
 *
 * \file HUD.h
 *
 * \brief Nakładka na ekran podczas grania (info o hp np.)
 *
 */


#ifndef HUD_H
#define HUD_H

#include "../include/SDL3/SDL.h"

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

    protected:

    private:
        int kill_stat;
        int death_stat;
};

#endif // HUD_H
