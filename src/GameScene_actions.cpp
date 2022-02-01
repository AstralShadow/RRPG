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
            process_action();
            break;

        default:
            print("Not parsing command: not implemented");
            break;
    }
}

