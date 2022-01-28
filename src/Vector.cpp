#include "Vector.hpp"
#include <cmath>
#include <stdexcept>


Vector::Vector(Point const& p) :
    _x(p.x), _y(p.y), _mode(M_POINT)
{}

Vector::Vector(SDL_Point const& p) :
    _x(p.x), _y(p.y), _mode(M_POINT)
{}

Vector::Vector(float direction, float length) :
    _direction(direction),
    _length(length),
    _mode(M_VECTOR)
{}

float Vector::direction() const
{
    ensure(M_VECTOR);
    return _direction;
}

void Vector::direction(float d)
{
    ensure(M_VECTOR);
    _direction = d;
    _mode = M_VECTOR;
}

float Vector::length() const
{
    ensure(M_VECTOR);
    return _length;
}

void Vector::length(float l)
{
    ensure(M_VECTOR);
    _length = l;
    _mode = M_VECTOR;
}

float Vector::x() const
{
    ensure(M_POINT);
    return _x;
}

void Vector::x(float x)
{
    ensure(M_POINT);
    _x = x;
    _mode = M_POINT;
}

float Vector::y() const
{
    ensure(M_POINT);
    return _y;
}

void Vector::y(float y)
{
    ensure(M_POINT);
    _y = y;
    _mode = M_POINT;
}

void Vector::ensure(VectorMode mode) const
{
    auto lacking = mode ^ (_mode & mode);

    switch(lacking)
    {
        default: return;

        case M_BOTH:
            throw std::logic_error(
                "Vector is not in any valid mode");

        case M_POINT:
        {
            _x = std::cos(_direction) * _length;
            _y = std::sin(_direction) * _length;

            _mode = M_BOTH;
            return;
        }

        case M_VECTOR:
        {
            _length = std::sqrt(_x * 2 + _y * 2);
            if(_length != 0)
                _direction = std::atan2(_y, _x);

            _mode = M_BOTH;
            return;
        }
    }
}
