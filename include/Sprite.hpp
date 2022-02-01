#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <string>
#include <chrono>
#include <map>
#include <SDL2/SDL_rect.h>

using std::string;
using std::chrono::milliseconds;
using std::map;


struct SpriteAnimationData
{
    int up = -1;
    int left = -1;
    int down = -1;
    int right = -1;
    uint8_t frames = 0;
    milliseconds delay = milliseconds(200);
};

struct Sprite
{
    string name;
    string texture;
    SDL_Point size{-1,-1};

    typedef string State;
    map<State, SpriteAnimationData> animation;
    SpriteAnimationData idle;
    SpriteAnimationData walking;
};

#endif
