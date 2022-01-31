#ifndef TILESET_HPP
#define TILESET_HPP

#include "globals.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;


struct Tileset
{
    Tileset() = default;
    ~Tileset() = default;
    
    id_t id;
    string name;
    string sprite;
    uint16_t width;
    uint16_t height;
};

#endif
