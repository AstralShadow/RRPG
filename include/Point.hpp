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
    
    Point& operator =(SDL_Point p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }

    Point& operator +=(Point p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point& operator -=(Point p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point& operator +=(float num)
    {
        x = x + num;
        y = y + num;
        return *this;
    }

    Point& operator /=(float num)
    {
        x = x / num;
        y = y / num;
        return *this;
    }

    Point& operator *=(float num)
    {
        x = x * num;
        y = y * num;
        return *this;
    }
};

extern Point operator + (Point const&,
                         Point const&);
extern Point operator - (Point const&,
                         Point const&);

#endif
