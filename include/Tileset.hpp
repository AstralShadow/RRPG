#ifndef TILESET_HPP
#define TILESET_HPP

#include "globals.hpp"
#include <string>
#include <vector>
#include <SDL2/SDL_rect.h>

using std::string;
using std::vector;


struct Tileset
{
    id_t id;
    string name;
    string sprite;
    SDL_Point size;
};

#endif