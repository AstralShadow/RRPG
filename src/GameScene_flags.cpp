#include "GameScene.hpp"
#include "Actions.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <memory>
#include <stdexcept>

using std::string;
using std::shared_ptr;
using std::runtime_error;


void GameScene::set_flag(shared_ptr<SetFlag> cmd)
{
    string flag = cmd->flag;
    _flags.insert(flag);
    auto story = _data->stories.find(_story);
    if(story == _data->stories.end())
        throw runtime_error
            ("Playing a story that don't exist.");

    auto arc_itr = story->second.events.find(flag);
    if(arc_itr == story->second.events.end())
        return;


    run_story_arc(&arc_itr->second);
}

void GameScene::
process_condition(shared_ptr<Condition> cmd)
{
    auto condition = _flags.find(cmd->flag);
    if(condition != _flags.end())
        run_story_arc(&cmd->actions);
    else
        run_story_arc(&cmd->alternative);
}

