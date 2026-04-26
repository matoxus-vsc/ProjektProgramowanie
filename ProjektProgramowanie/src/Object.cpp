#include "../include/SDL3_image/SDL_image.h"

#include "Object.h"
#include "object_init.h"

Object::Object()
{
    texture = nullptr;
    texture_height = 0;
    texture_width = 0;
}
Object::Object(float x, float y, const char* p)
{
    texture_load(p);
    pos.arg_set(x, y);
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

SDL_Texture* Object::texture_get()
{
    return texture;
}
int Object::width_get()
{
    return texture_width;
}
int Object::height_get()
{
    return texture_height;
}
Text::Text()
{
    texture = nullptr;
    texture_width = 0;
    texture_height = 0;

    str = nullptr;
    font = nullptr;
    font_color = {0, 0, 0};
}
Text::~Text()
{
    destroy();
}
bool Text::text_load(const char* strr, TTF_Font* fontt, SDL_Color font_colorr)
{

    destroy();

    SDL_Surface* text_surface = TTF_RenderText_Solid(fontt, strr, strlen(strr), font_colorr);
    if(text_surface==nullptr)
    {
        SDL_Log("Unable to render text %s! %s\n", str, SDL_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(t1.renderer_get(), text_surface);
        if(texture == nullptr )
        {
            SDL_Log("Unable to create texture from loaded pixels!\n%s", SDL_GetError());
        }
        else
        {

            texture_width = text_surface->w;
            texture_height = text_surface->h;
            str = strr;
            font = fontt;
            font_color = font_colorr;
        }


        SDL_DestroySurface(text_surface);
    }
    return texture!=nullptr;
}
void Object::position_update(Vec2f update)
{
    pos.arg_set(update.x, update.y);
}
Vec2f Object::position_get()
{
    return pos;
}

