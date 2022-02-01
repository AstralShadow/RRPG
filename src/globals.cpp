#include "globals.hpp"
#include <SDL2/SDL_pixels.h>


bool operator < (SDL_Color const& a,
                 SDL_Color const& b)
{
    if(a.r != b.r) return a.r < b.r;
    if(a.g != b.g) return a.g < b.g;
    if(a.b != b.b) return a.b < b.b;
    return a.a < b.a;
}
