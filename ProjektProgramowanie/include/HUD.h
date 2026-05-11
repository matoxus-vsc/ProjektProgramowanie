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
        /** \brief Konstruktor
         *
         *
         */
        HUD();

        /** \brief Destruktor
         *
         *
         */
        ~HUD();

        /** \brief Renderowanie fps liczby fps
         *
         * \param rend_frame Uint64& - klatka
         * \return void
         *
         */
        void fps_render(Uint64 &rend_frame);

        /** \brief Renderowanie panelu dolnego
         *
         * \return void
         *
         */
        void banner_bottom_render();

    protected:

    private:
        int kill_stat;
        int death_stat;
};

#endif // HUD_H
