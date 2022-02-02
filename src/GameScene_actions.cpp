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
        case Command::MAP:
            set_map(cmd->name);
            break;

        case Command::STORY:
            set_story(cmd->name);
            _wait = false;
            break;

        case Command::SPAWN:
            spawn_entity(cmd->name,
                         cmd->pos,
                         cmd->state);
            break;

        case Command::STATE:
            set_entity_state(cmd->name, cmd->state);
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

void GameScene::spawn_entity(string name, 
                             SDL_Point pos,
                             string state)
{
    auto metadata_itr = _data->entities.find(name);
    if(metadata_itr == _data->entities.end())
    {
        print("Can not find entity: ", name);
        throw runtime_error("Can not find entity.");
    }
    auto& metadata = metadata_itr->second;

    auto sprite = _data->sprites.find(metadata.sprite);
    if(sprite == _data->sprites.end())
    {
        print("Can not find sprite: ", metadata.sprite);
        throw runtime_error("Can not find sprite.");
    }

    auto& entity = _entities[name];
    entity.name = name;
    entity.pos = pos;
    entity.sprite = &(sprite->second);
    set_entity_state(name, state);

    print("Spawning entity: ", name);
}

void GameScene::set_entity_state(string name,
                                 string state)
{
    auto entity = _entities.find(name);
    if(entity == _entities.end())
    {
        print("Can not set state: Entity not in scene.");
        return;
    }

    auto sprite = entity->second.sprite;
    auto sprite_state = sprite->animation.find(state);
    if(sprite_state == sprite->animation.end())
    {
        print("Entity state not in sprite animations.");
        print("Entity: ", name, " State: ", state);
        throw std::runtime_error("Invalid entity state");
    }

    entity->second.state = state;
}

