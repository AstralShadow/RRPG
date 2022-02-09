#include "Point.hpp"

Point operator + (Point const& a,
                  Point const& b)
{
    return {a.x + b.x, a.y + b.y};
}

Point operator - (Point const& a,
                  Point const& b)
{
    return {a.x - b.x, a.y - b.y};
}
