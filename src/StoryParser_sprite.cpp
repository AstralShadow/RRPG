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

    if(args[0] == "animation" || args[0] == "anim")
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
    
    if(args.size() < 3)
    {
        print("Throwing exception at unfinished line:");
        print(line);
        throw std::runtime_error
            ("Finish commands that you start.");
    }

    string state = args[1];
    int8_t frame_id = -1;

    auto& ani_data = _sprites[_target].animation[state];


    if(args[2] == "frames")
    {
        if(args.size() < 3)
            print("Unfinished command: ", line);
        else
        {
            uint8_t frames = stoi(args[3]);
            ani_data.frames = frames;
        }
        return;
    }

    if(args[2] == "speed")
    {
        if(args.size() < 3)
            print("Unfinished command: ", line);
        else
        {
            uint8_t speed = stoi(args[3]);
            ani_data.speed = speed;
        }
        return;
    }

    auto pos = explode('.', args[2], 1);
    if(pos.size() == 1)
        frame_id = stoi(pos[0]);
    else
    {
        int height = _sprites[_target].size.y;
        if(height < 1)
            throw std::runtime_error
                ("You have to define sprite size"
                " to use position notation on it.");

        frame_id = stoi(pos[0]) + stoi(pos[1]) * height;
    }

    
    if(args.size() == 3)
    {
        ani_data.up = frame_id;
        ani_data.left = frame_id;
        ani_data.down = frame_id;
        ani_data.right = frame_id;
    }
    else if(args[3] == "up")
        ani_data.up = frame_id;
    else if(args[3] == "left")
        ani_data.left = frame_id;
    else if(args[3] == "down")
        ani_data.down = frame_id;
    else if(args[3] == "right")
        ani_data.right = frame_id;
    else
        print("Could not parse sprite command: ", line);
}

