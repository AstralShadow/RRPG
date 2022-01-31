#include "StoryParser.hpp"
#include "str_utils.hpp"
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"
#include <stdexcept>


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
        _sprites[_target].texture = args[1];
    else if(args[0] == "");
    else
        print("Unknown command: ", line);
}

