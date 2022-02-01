#include "GameScene.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <stdexcept>

using std::string;
using std::runtime_error;

void GameScene::set_map(string name)
{
    auto map = _data->maps.find(name);
    if(map == _data->maps.end())
    {
        print("Can not find map: ", name);
        throw runtime_error("Can not find map.");
    }
    print("Preparing map: ", name);
    _map = &(map->second);
    _tilesets.empty();

    for(auto pair : _map->tilesets)
    {
        auto const& key = pair.first;
        auto const& name = pair.second;
        auto& cached = _tilesets[key];
        auto tileset = _data->tilesets.find(name);
        if(tileset == _data->tilesets.end())
        {
            print("Can not find tileset ", name);
            throw runtime_error("Can not find tileset.");
        }

        auto uri = _data->assets_dir +
            tileset->second.texture;
        auto size = tileset->second.size;
        auto texture = _engine->get_texture(uri);

        cached.first = texture;
        cached.second = size;
    }
}


