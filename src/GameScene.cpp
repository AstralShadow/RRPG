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
    _story(main_story),
    _linestack()
{
    ActionPointer beginning {
        &(_data->stories[_story].actions),
        _data->stories[_story].actions.begin()
    };
    _linestack.push(beginning);
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

void GameScene::render(SDL_Renderer* renderer)
{

}

void GameScene::process(SDL_Event const& e)
{
    if(e.type == SDL_MOUSEBUTTONDOWN)
        process_action();
}

void GameScene::process_action()
{
    if(_linestack.size() == 0)
    {
        print("Story finished.");
        return;
    }

    auto& act_itr = _linestack.top().act;
    auto& act = *act_itr;
    auto& line = *(_linestack.top().line);

    switch(act->type())
    {
        default:
            print("Not parsing action of type ",
                  act->type_name());
            break;
    }

    if(++act_itr == line.end())
        _linestack.pop();
}
