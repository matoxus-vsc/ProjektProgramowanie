/** \author Igor Solny
 *
 * \file structs.h
 *
 * \brief Globalne struktury, enumy, itd. używane w innych plikach
 *
 */


#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

enum Game_state
{
    PLAYING
};

struct Vec2i
{
    int x;
    int y;

    void arg_set(int xx, int yy)
    {
        x = xx;
        y = yy;
    }
    Vec2i(){};
    Vec2i(int xx, int yy)
    {
        arg_set(xx, yy);
    }
    Vec2i(const Vec2i& p)
    {
        arg_set(p.x, p.y);
    }
};
struct Vec2f
{
    float x;
    float y;

    void arg_set(float xx, float yy)
    {
        x = xx;
        y = yy;
    }
    Vec2f(){};
    Vec2f(float xx, float yy)
    {
        arg_set(xx, yy);
    }
    Vec2f(const Vec2f& p)
    {
        arg_set(p.x, p.y);
    }
};

#endif // STRUCTS_H_INCLUDED
