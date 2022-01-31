#ifndef MAP_HPP
#define MAP_HPP

#include "Tileset.hpp"
#include <string> 
#include <vector> 
#include <map>
#include <SDL2/SDL_rect.h>


struct Map
{
    struct TileData;

    id_t id;
    string name;
    SDL_Point size;
    map<char, string> tilesets;
    vector<TileData> data;
};

struct Map::TileData
{
    TileData(char t, uint16_t id) :
        tileset(t),
        mode(mode_id)
    {
        tile.id = id;
    }

    TileData(char t, uint8_t x, uint8_t y) :
        tileset(t),
        mode(mode_pos)
    {
        tile.pos.x = x;
        tile.pos.y = y;
    }

    ~TileData() = default;

    char tileset;

    union {
        struct Point{
            uint8_t x;
            uint8_t y;
        } pos;
        uint16_t id;
    } tile;
    
    enum : uint8_t
    {
        mode_pos,
        mode_id
    } mode;
};

#endif