#ifndef TILESET_HPP
#define TILESET_HPP

#include <string>
#include <vector>
#include <SDL2/SDL_rect.h>

using std::string;
using std::vector;


struct Tileset
{
    string name;
    string texture;
    SDL_Point size;
};

#endif
