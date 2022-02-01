#include "GameScene.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <SDL2/SDL_events.h>

using std::chrono::milliseconds;
using std::string;

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
