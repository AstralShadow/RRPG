#include "GameScene.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <SDL2/SDL_events.h>

using std::chrono::milliseconds;
using std::string;


GameScene::GameScene(Engine* engine, string main_story) :
    Scene(engine),
    _data(&engine->get_story()),
    _story(),
    _linestack(),
    _map(),
    _tilesets(),
    _camera_offset{0, 0}
{
    set_story(main_story);
}

GameScene::~GameScene()
{ }

void GameScene::on_enter()
{
    _wait = false;
}

void GameScene::tick(duration_t progress)
{
    update_entity_frames(progress);
    while(!_wait)
    {
        process_action();
    }
}

void GameScene::update_entity_frames(duration_t progress)
{
    for(auto& pair : _entities)
    {
        auto& entity = pair.second;
        entity.animation_progress += progress;
    }
}

void GameScene::process(SDL_Event const& e)
{
    if(e.type == SDL_MOUSEBUTTONUP)
    {
        if(!_dragging && SDL_BUTTON_LEFT)
            _wait = false;
        if(!_dragging && SDL_BUTTON_RIGHT)
            _zoom = 2.5;
        _dragging = false;
    }
    if(e.type == SDL_MOUSEMOTION)
    {
        if(e.motion.state & SDL_BUTTON_LMASK)
        {
            _dragging = true;
            _camera_offset.x += e.motion.xrel;
            _camera_offset.y += e.motion.yrel;
        }
        if(e.motion.state & SDL_BUTTON_RMASK)
        {
            _dragging = true;
            _zoom += e.motion.xrel / 100.0;
            print(e.motion.xrel, " ", _zoom);
            if(_zoom < 1.0)
                _zoom = 1.0;
            if(_zoom > 5.0)
                _zoom = 5.0;
        }
    }
        
}

void GameScene::process_action()
{
    if(_linestack.size() == 0)
    {
        print("Story finished.");
        return;
    }

    auto& act_itr = _linestack.top().act;
    auto act = *act_itr;
    auto& line = *(_linestack.top().line);

    act_itr++;
    if(act_itr == line.end())
        _linestack.pop();

    _wait = true;

    switch(act->type())
    {
        case Action::action_command:
            process_command(std::static_pointer_cast
                            <Command>(act));
            break;

        default:
            print("Not parsing action of type ",
                  act->type_name());
            break;
    }
}
