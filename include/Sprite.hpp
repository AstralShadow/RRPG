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
    /** Description
     * Direction falues are tile ids in sprite.
     * Frames means how many tiles to count in animation
     * Frames in one animation must be consecutive.
     * For example: up=5 frames=3 means up takes 5, 6, 7
     * Speed = FPS
     */
    uint16_t up = -1;
    uint16_t left = -1;
    uint16_t down = -1;
    uint16_t right = -1;
    uint16_t frames = 1;
    uint16_t speed = 1;
};

struct Sprite
{
    string name;
    string texture;
    SDL_Point size{-1,-1};

    typedef string State;
    map<State, SpriteAnimationData> animation;
};

#endif
