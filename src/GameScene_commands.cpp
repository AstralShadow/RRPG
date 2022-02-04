#include "GameScene.hpp"
#include "Actions.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <stdexcept>
#include <cmath>

using std::string;
using std::runtime_error;
using std::sqrt;
using std::chrono::duration_cast;
using std::chrono::milliseconds;


void GameScene::process_command(shared_ptr<Command> cmd)
{
    switch(cmd->command)
    {
        case Command::MAP:
            set_map(cmd->name);
            break;

        case Command::STORY:
            set_story(cmd->name);
            break;

        case Command::SPAWN:
            spawn_entity(cmd->name,
                         cmd->pos,
                         cmd->state);
            break;

        case Command::MOVE:
            move_entity(cmd->name, cmd->pos);
            break;

        case Command::STATE:
            set_entity_state(cmd->name,
                             cmd->state);
            break;

        case Command::LOOK:
            set_entity_direction(cmd->name, cmd->state);
            break;

        case Command::REMOVE:
            remove_entity(cmd->name);
            break;

        case Command::SLEEP:
            sleep(milliseconds(std::stoi(cmd->name)));
            break;

        case Command::WAIT:
            wait_entity_motion(cmd->name);
            break;

        default:
            print("Unknown command: ", cmd->command);
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

    auto* arc = &(itr->second.actions);
    run_story_arc(arc);
    _story = name;
}

void GameScene::
run_story_arc(vector<shared_ptr<Action>>* arc)
{
    if(arc->size())
    {
        ActionPointer beginning {
            arc,
            arc->begin()
        };
        _linestack.push(beginning);
    }
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
    _entities.empty();
    _motions.empty();

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
        print("requested by entity: ", name);
        throw runtime_error("Can not find sprite.");
    }

    auto& entity = _entities[name];
    entity.name = name;
    entity.pos = pos;
    entity.sprite = &(sprite->second);
    entity.direction = Direction::DOWN;
    set_entity_state(name, state);
    entity.texture = _engine->get_texture
        (_data->assets_dir + sprite->second.texture);

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
    entity->second.animation_progress = 
       entity->second.animation_progress.zero();
}

void GameScene::set_entity_direction(string name,
                                     string direction)
{
    auto entity = _entities.find(name);
    if(entity == _entities.end())
    {
        print("Can not set direction: Entity not here.");
        return;
    }

    auto& value = entity->second.direction;

    if(direction == "left")
        value = Direction::LEFT;
    else if(direction == "right")
        value = Direction::RIGHT;
    else if(direction == "down")
        value = Direction::DOWN;
    else if(direction == "up")
        value = Direction::UP;
    else print("Unknown direction: ", direction);

}

void GameScene::remove_entity(string name)
{
    _entities.erase(name);
}

void GameScene::move_entity(string name, SDL_Point pos)
{
    auto entity = _entities.find(name);
    if(entity == _entities.end())
    {
        print("Can not move entity that doesn't exist.");
        return;
    }

    Motion motion;
    motion.entity = name;
    motion.progress = duration_t(0);

    Point delta{
        pos.x - entity->second.pos.x,
        pos.y - entity->second.pos.y
    };
    int duration =
        sqrt (delta.x * delta.x + delta.y * delta.y)
        * 1000 / 3;
    motion.length = duration_t(duration);
    motion.start.x = entity->second.pos.x;
    motion.start.y = entity->second.pos.y;
    motion.end.x = pos.x;
    motion.end.y = pos.y;

    _motions.push_front(motion);
}

void GameScene::wait_entity_motion(string name)
{
    duration_t max(0);
    for(auto& motion : _motions)
    {
        if(motion.entity != name) continue;
        auto ETA = motion.length - motion.progress;
        if(ETA > max)
            max = ETA;
    }

    sleep(duration_cast<milliseconds>(max));
}

void GameScene::finish_motion(Motion& motion)
{
    _entities[motion.entity].pos.x = motion.end.x;
    _entities[motion.entity].pos.y = motion.end.y;
    print("Finished motion to ", motion.end.x, 
            ":", motion.end.y);
}
