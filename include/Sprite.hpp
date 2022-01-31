#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <string>
#include <SDL2/SDL_rect.h>
using std::string;

struct SpriteAnimationData
{
    int8_t up = -1;
    int8_t left = -1;
    int8_t down = -1;
    int8_t right = -1;
    uint8_t animation = 0;
};

struct Sprite
{
    string texture;
    SDL_Point size;

    SpriteAnimationData idle;
    SpriteAnimationData walking;
};

#endif
