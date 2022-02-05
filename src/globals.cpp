#include "globals.hpp"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>

static std::random_device rd;
static const uint32_t seed = rd();
std::mt19937 mt_generator(seed);


bool operator < (SDL_Color const& a,
                 SDL_Color const& b)
{
    if(a.r != b.r) return a.r < b.r;
    if(a.g != b.g) return a.g < b.g;
    if(a.b != b.b) return a.b < b.b;
    return a.a < b.a;
}

SDL_Rect operator * (SDL_Rect const& input, float num)
{
    SDL_Rect output = input;
    output.x *= num;
    output.y *= num;
    output.w *= num;
    output.h *= num;
    return output;
}
