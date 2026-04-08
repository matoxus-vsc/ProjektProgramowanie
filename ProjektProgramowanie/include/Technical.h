/** \author Igor Solny
 *
 * \file Technical.h
 *
 * \brief Technikalia SDL
 */

#ifndef TECHNICAL_H
#define TECHNICAL_H

#include "../include/SDL3/SDL.h"

enum Errors
{
    NO_ERROR = 0x000,
    ERR_SDL_INIT = 0x001,
    ERR_SDL_CREATE_WINDOW = 0x002,
    ERR_SDL_GET_WINDOW_SIZE = 0x003,
    ERR_SDL_CREATE_RENDERER = 0x004
};

class Technical
{
    public:
        Technical();
        ~Technical();

        bool init();
        bool media();
        void close();

        SDL_Window* window_get();
        int window_width_get();
        int window_height_get();

        SDL_Renderer* renderer_get();

    protected:

    private:
        SDL_Window* window;
        int window_width;
        int window_height;

        SDL_Renderer* renderer;

        Errors err_code;

};

#endif // TECHNICAL_H
