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

void GameScene::tick(milliseconds)
{
    while(!_wait)
    {
        process_action();
    }
}

void GameScene::process(SDL_Event const& e)
{
    if(e.type == SDL_MOUSEBUTTONUP)
    {
        if(!_dragging)
            _wait = false;
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
