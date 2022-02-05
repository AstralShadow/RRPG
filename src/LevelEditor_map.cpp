#include "LevelEditor.hpp"


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
