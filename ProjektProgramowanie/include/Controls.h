#ifndef CONTROLS_H
#define CONTROLS_H

#include "SDL3/SDL.h"

#include "structs.h"

class Mouse
{
    public:
        Mouse();
        ~Mouse();

        void position_set(float x, float y);

        void mouse_handler(SDL_Event* e, Game_state s);

        Vec2f position_get();

        void position_print();

    protected:

    private:
        Vec2f position;
};


#endif // CONTROLS_H
