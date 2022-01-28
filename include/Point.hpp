#ifndef POINT_HPP
#define POINT_HPP

#include <SDL2/SDL_rect.h>

struct Point
{
    float x;
    float y;

    operator SDL_Point() const
    {
        SDL_Point p;
        p.x = x + 0.5;
        p.y = y + 0.5;
        return p;
    }
};

#endif
