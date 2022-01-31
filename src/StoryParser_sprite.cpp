#include "StoryParser.hpp"
#include "str_utils.hpp"
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"
#include <stdexcept>

using std::stoi;


void StoryParser::
begin_sprite_state(string& line,
                   vector<string>& args)
{
    if(args.size() < 2)
        throw std::runtime_error
            ("Can not load unnamed sprite");

    string name = explode(' ', line, 1)[1];

    auto &sprite = _sprites[name];
    sprite.name = name;
    
    _target = name;
    _state = state_sprite;
    print("Loading sprite ", name);
}

void StoryParser::
parse_sprite_command(string& line,
                     vector<string>& args)
{
    if(args[0] == "file")
    {
        _sprites[_target].texture = args[1];
        return;
    }

    if(args[0] == "size" && args.size() == 3)
    {
        _sprites[_target].size.x = stoi(args[1]);
        _sprites[_target].size.y = stoi(args[2]);
        return;
    }

    if(args[0] == "idle" || args[0] == "walking")
    {
        parse_sprite_animation_command(line, args);
        return;
    }

    print("Unknown command: ", line);
}

void StoryParser::
parse_sprite_animation_command(string& line,
                               vector<string>& args)
{
    int argc = args.size();
    if(argc < 2 || (args[1] == "frames" && argc != 3))
    {
        print("Unfinished command: ", line);
        return;
    }
        

    string mode = args[0];
    int8_t frame_id = -1;


    if(mode != "idle" && mode != "walking")
    {
        string msg = "Unknown sprite mode: " + mode;
        throw std::runtime_error(msg);
    }

    SpriteAnimationData* ani_data = nullptr;
    if(mode == "idle")
        ani_data = &(_sprites[_target].idle);
    else 
        ani_data = &(_sprites[_target].walking);


    if(args[1] == "frames")
    {
        uint8_t frames = stoi(args[2]);
        ani_data->frames = frames;
        return;
    }

    auto pos = explode('.', args[1], 1);
    if(pos.size() == 1)
        frame_id = stoi(args[1]);
    else
    {
        int height = _sprites[_target].size.y;
        if(height < 1)
            throw std::runtime_error
                ("You have to define sprite size"
                " to use position notation on it.");

        frame_id = stoi(pos[0]) + stoi(pos[1]) * height;
    }

    
    if(argc == 2)
    {
        ani_data->up = frame_id;
        ani_data->left = frame_id;
        ani_data->down = frame_id;
        ani_data->right = frame_id;
    }
    else if(args[2] == "up")
        ani_data->up = frame_id;
    else if(args[2] == "left")
        ani_data->left = frame_id;
    else if(args[2] == "down")
        ani_data->down = frame_id;
    else if(args[2] == "right")
        ani_data->right = frame_id;
    else
        print("Could not parse sprite command: ", line);
}

