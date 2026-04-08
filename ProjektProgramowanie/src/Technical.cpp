#include "Technical.h"

#include <iostream>

Technical::Technical()
{
    window = nullptr;
    window_width = 0;
    window_height = 0;

    renderer = nullptr;

    err_code = NO_ERROR;

    if(!init())
    {
        std::cerr<<"\nFailed to initialize game!\nError Code:"<<err_code<<'\n';
        exit(1);
    }
}

Technical::~Technical()
{
    //dtor
}

bool Technical::init()
{
    bool success = true;

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr<<"SDL_Init(SDL_INIT_VIDEO) failed!\n"<<SDL_GetError();
        err_code = ERR_SDL_INIT;
        success = false;
    }
    else
    {
        window = SDL_CreateWindow("SC2D", 800, 640, 0);
        if(window == nullptr)
        {
            std::cerr<<"SDL_CreateWindow(\'SC2D\', 800, 640, 0) failed!\n"<<SDL_GetError();
            err_code = ERR_SDL_CREATE_WINDOW;
            success = false;
        }
        else
        {
            if(!SDL_GetWindowSize(window, &window_width, &window_height))
            {
                std::cerr<<"SDL_GetWindowSize(window, window_width, window_height) failed!\n"<<SDL_GetError();
                err_code = ERR_SDL_GET_WINDOW_SIZE;
                success = false;
            }
            else
            {
                renderer = SDL_CreateRenderer(window, nullptr);
                if(renderer==nullptr)
                {
                    std::cerr<<"SDL_CreateRenderer(window, nullptr) failed!\n"<<SDL_GetError();
                    err_code = ERR_SDL_CREATE_RENDERER;
                    success = false;
                }
            }
        }
    }

    return success;
}

SDL_Window* Technical::window_get()
{
    return window;
}
int Technical::window_height_get()
{
    return window_height;
}
int Technical::window_width_get()
{
    return window_width;
}
SDL_Renderer* Technical::renderer_get()
{
    return renderer;
}


