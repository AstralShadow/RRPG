#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstdint>
#include <SDL2/SDL_rect.h>
#include "Point.hpp"


/** A 2d geometry vector */
class Vector
{
    public:
        Vector(Point const&);
        Vector(SDL_Point const&);
        Vector(float direction, float length);
        ~Vector() = default;

        float direction() const;
        float length() const;
        float x() const;
        float y() const;

        void x(float);
        void y(float);
        void direction(float);
        void length(float);

        operator SDL_Point() const
        {
            SDL_Point p;
            p.x = x() + 0.5;
            p.y = y() + 0.5; 
            return p;
        }

        operator Point() const
        {
            return Point{ x(), y() };
        }

    private:
        mutable float _direction;
        mutable float _length;
        mutable float _x;
        mutable float _y;

        enum VectorMode : uint8_t {
            M_VECTOR = 1,
            M_POINT = 2,
            M_BOTH = 3
        };
        mutable VectorMode _mode;

        void ensure(VectorMode) const;
};

// Vector a{std::atan2(4, 3), 10};
// std::cout << a.x() << std::endl << a.y() << std::endl;

#endif
