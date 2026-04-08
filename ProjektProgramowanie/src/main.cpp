#include <iostream>
#include "../include/SDL3/SDL.h"
#include "../include/SDL3/SDL_main.h"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    std::cout << "Hello World!\n";

    SDL_Quit();
}

