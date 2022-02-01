#include "GameScene.hpp"
#include "Actions.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <stdexcept>

using std::string;
using std::runtime_error;


void GameScene::process_command(shared_ptr<Command> cmd)
{
    switch(cmd->command)
    {
        case Command::map:
            set_map(cmd->name);
            break;

        case Command::story:
            set_story(cmd->name);
            _wait = false;
            break;

        case Command::spawn:
            spawn_entity(cmd->name, cmd->pos);
            break;

        default:
            print("Not parsing command: not implemented");
            break;
    }
}


void GameScene::set_story(string name)
{
    auto itr = _data->stories.find(name);
    if(itr == _data->stories.end())
    {
        print("Story not found ", name);
        throw std::runtime_error("Can not load story.");
    }
    
    print("Playing story: ", name);

    _linestack.empty();
    _map = nullptr;

    auto* actions = &(itr->second.actions);
    ActionPointer beginning {
        actions,
        actions->begin()
    };
    _linestack.push(beginning);
    _story = name;
}

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

void GameScene::spawn_entity(string name, SDL_Point pos)
{
    print("Spawning entity");

}

