#include "../include/SDL3_image/SDL_image.h"

#include "Object.h"

#include "object_init.h"
Object::Object()
{
    texture = nullptr;
    texture_height = 0;
    texture_width = 0;

}

Object::~Object()
{
    destroy();
}

bool Object::texture_load(const char* path)
{
    destroy();

    SDL_Surface* surface = IMG_Load(path);
    if(surface == nullptr )
    {
        SDL_Log("Unable to load image %s! %s\n", path, SDL_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface( t1.renderer_get(), surface );
        if(texture == nullptr )
        {
            SDL_Log("Unable to create texture from loaded pixels!\n%s", SDL_GetError());
        }
        else
        {

            texture_width = surface->w;
            texture_height = surface->h;
        }


        SDL_DestroySurface( surface );
    }

    return texture != nullptr;
}

void Object::destroy()
{
    if(texture!=nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;

        texture_height = 0;
        texture_width = 0;
    }
}



void Object::render(float x, float y, double angle, SDL_FPoint* center)
{
    SDL_FRect to_render
    {
        x,
        y,
        static_cast<float>(texture_width),
        static_cast<float>(texture_height)
    };


    SDL_RenderTextureRotated(t1.renderer_get(), texture, nullptr, &to_render, angle, center, SDL_FLIP_NONE);
}

int Object::width_get()
{
    return texture_width;
}
int Object::height_get()
{
    return texture_height;
}

