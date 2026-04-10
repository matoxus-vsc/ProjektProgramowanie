#include "Technical.h"
#include "object_init.h"

#include <iostream>

Technical::Technical()
{
    window = nullptr;
    window_width = 0;
    window_height = 0;

    renderer = nullptr;

    err_code = NO_ERROR;
    if(!init(t1))
    {
        SDL_Log("Failed to initialize SDL, Error_technical code:%i %s",err_code, SDL_GetError());
        exit(1);
    }
}

Technical::~Technical()
{
    //dtor
}

bool init(Technical& t)
{
    bool success = true;

    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed! \n%s", SDL_GetError());
        t.err_code = ERR_SDL_INIT;
        success = false;
    }
    else
    {
        t.window = SDL_CreateWindow("SC2D", 800, 640, 0);
        if(t.window == nullptr)
        {
            SDL_Log("SDL_CreateWindow(\'SC2D\', 800, 640, 0) failed!\n%s", SDL_GetError());
            t.err_code = ERR_SDL_CREATE_WINDOW;
            success = false;
        }
        else
        {
            if(!SDL_GetWindowSize(t.window, &t.window_width, &t.window_height))
            {
                SDL_Log("SDL_GetWindowSize(t.window, &t.window_width, &t.window_height) failed!\n%s", SDL_GetError());
                t.err_code = ERR_SDL_GET_WINDOW_SIZE;
                success = false;
            }
            else
            {
                t.renderer = SDL_CreateRenderer(t.window, nullptr);
                if(t.renderer==nullptr)
                {
                    SDL_Log("SDL_CreateRenderer(t.window, nullptr)\n%s", SDL_GetError());
                    t.err_code = ERR_SDL_CREATE_RENDERER;
                    success = false;
                }
            }
        }
    }

    return success;
}

void close(Technical& t)
{
    if(t.renderer!=nullptr)
    {
        SDL_DestroyRenderer(t.renderer);
        t.renderer = nullptr;
    }

    if(t.window!=nullptr)
    {
        SDL_DestroyWindow(t.window);
        t.window = nullptr;
    }

    SDL_Quit();
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


