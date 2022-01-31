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

    print("Sprite loading not implemented.");
    return;

    auto id = _stories.size();
    string name = explode(' ', line, 1)[1];

    auto &entity = _entities[name];
    entity.name = name;
    entity.id = id;
    
    _target = name;
    _state = state_entity;
    print("Loading character ", name);
}

void StoryParser::
parse_sprite_command(string& line,
                     vector<string>& args)
{
    if(args[0] == "file")
        _sprites[_target].texture = args[1];
    else
        print("Unknown command: ", line);
}

