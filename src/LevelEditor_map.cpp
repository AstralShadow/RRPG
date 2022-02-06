#include "LevelEditor.hpp"
#include "Map.hpp"
#include "Engine.hpp"
#define ENABLE_PRINTING 1
#include "print.hpp"
#include <fstream>
#include <stdexcept>

using std::ofstream;


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


void LevelEditor::Map::save(string uri, string name)
{
    auto chars = map_tileset_chars();
    ofstream file(uri);
    file << "#map " << name << "\n    ";
    file << ":size " << _size.x << " "
                     << _size.y << "\n";

    for(auto pair : chars)
    {
        file << "    :tileset ";
        file << pair.first->name << " "
             << pair.second << "\n";
    }
    file << "\n";

    for(int y = 0; y < _size.y; y++)
    {
        for(int x = 0; x < _size.x; x++)
        {
            file << ' ';
            auto& tile = at(x, y);
            if(tile.empty)
            {
                file << '.';
                continue;
            }
            file << chars[tile.tileset];
            file << tile.x << '.' << tile.y;
        }
        file << "\n";
    }
}

map<Tileset*, char> LevelEditor::Map::map_tileset_chars()
{
    map<Tileset*, char> chars;
    char next = 'a';
    
    for(int i = 0; i < _size.x * _size.y; ++i)
    {
        auto& tile = (*this)[i];
        auto itr = chars.find(tile.tileset);
        if(itr != chars.end())
            continue;

        chars[tile.tileset] = next;
        if(next == 'z')
            next = 'A';
        else if(next == 'Z')
        {
            throw std::runtime_error
                ("We all knew that's gonna happen."
                "\nOur map format supports limited "
                "number of tilesets used per map.\n"
                "Still, congratulations for using "
                "over the soft-limited 56 tilesets "
                "in a single map.");
        }
        else next++;
    }

    return chars;
}


