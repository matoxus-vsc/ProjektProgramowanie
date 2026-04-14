#include "Controls.h"

Mouse::Mouse()
{
    position_set(0 ,0);
}

Mouse::~Mouse()
{
    //dtor
}
void Mouse::position_set(float x, float y)
{
    position.arg_set(x, y);
}
void Mouse::mouse_handler(SDL_Event* e, Game_state s)
{
    float x,y;
    SDL_GetMouseState(&x, &y);

    position_set(x, y);

    position_print();
}
Vec2f Mouse::position_get()
{
    return position;
}
void Mouse::position_print()
{
    SDL_Log("MOUSE:X:%0.f Y:%0.f", position.x, position.y);
}
