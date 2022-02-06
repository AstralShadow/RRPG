#include "LevelEditor.hpp"
#include "Map.hpp"
#include "Engine.hpp"
#define ENABLE_PRINTING 1
#include "print.hpp"


LevelEditor::Map::Map()
{
    size_t count = _actual_size.x * _actual_size.y;
    _data = new TileData[count];
}

LevelEditor::Map::TileData&
LevelEditor::Map::operator [](int pos)
{
    return at(pos % _size.x, pos / _size.x);
}

LevelEditor::Map::TileData&
LevelEditor::Map::at(int x, int y)
{
    return _data[x + y * _actual_size.x];
}

void LevelEditor::Map::size(SDL_Point size)
{
    if(size.x <= _actual_size.x &&
        size.y <= _actual_size.y)
    {
        _size = size;
        return;
    }
    
    resize(size);
}

void LevelEditor::Map::resize(SDL_Point size)
{
    auto data = new TileData[size.x * size.y];
    for(int x = 0; x < _size.x; x++)
    {
        for(int y = 0; y < _size.y; y++)
        {
            auto& tile = data[x + y * size.x];
            tile = at(x, y);
        }
    }

    delete[] _data;
    _data = data;
    _size = size;
    _actual_size = size;
}

void LevelEditor::Map::load_map(::Map* map, Engine* core)
{
    print("Loadin map: ", map->name);
    resize(map->size);
    
    auto& tilesets = core->get_story().tilesets;

    SDL_Point pos {0, 0};
    for(auto data : map->data)
    {
        if(pos.x >= map->size.x)
        {
            pos.x = 0;
            pos.y++;
        }

        if(data.mode == ::Map::TileData::mode_empty)
        {
            pos.x++;
            continue;
        }

        auto& tile = at(pos.x, pos.y);
        
        auto& tileset = tilesets.find
                (map->tilesets[data.tileset])->second;

        tile.empty = false;
        tile.tileset = &tileset;

        switch(data.mode)
        {
            case ::Map::TileData::mode_pos:
                tile.x = data.tile.pos.x;
                tile.y = data.tile.pos.y;
                break;

            case ::Map::TileData::mode_id:
                tile.x = data.tile.id % tileset.size.x;
                tile.y = data.tile.id / tileset.size.x;
                break;

            default:
                tile.empty = true;
                break;
        }

        pos.x++;
    }
}

