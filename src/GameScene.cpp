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
    _map(),
    _tilesets(),
    _linestack(),
    _camera_offset{0, 0}
{
    set_story(main_story);
}

GameScene::~GameScene()
{ }

void GameScene::on_enter()
{
    process_action();
}

void GameScene::tick(milliseconds progress)
{

}

void GameScene::process(SDL_Event const& e)
{
    if(e.type == SDL_MOUSEBUTTONUP)
    {
        if(!_dragging)
            process_action();
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

    if(_linestack.top().act == _linestack.top().line->begin())
        print("We're at the beginning.");
    if(_linestack.top().act == _linestack.top().line->end())
        print("We're at the end.");

    auto& act_itr = _linestack.top().act;
    auto act = *act_itr;
    auto& line = *(_linestack.top().line);

    act_itr++;
    if(act_itr == line.end())
        _linestack.pop();

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
