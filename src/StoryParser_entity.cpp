#include "StoryParser.hpp"
#include "str_utils.hpp"
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"
#include <stdexcept>


void StoryParser::
begin_entity_state(string& line,
                   vector<string>& args)
{
    if(args.size() < 2)
        throw std::runtime_error
            ("Can not load unnamed entity");

    string name = explode(' ', line, 1)[1];

    auto &entity = _entities[name];
    entity.name = name;
    
    _target = name;
    _state = state_entity;
    print("Loading entity ", name);
}

void StoryParser::
parse_entity_command(string& line,
                     vector<string>& args)
{
    if(args[0] == "sprite")
        _entities[_target].sprite =
            explode(' ', line, 1)[1];
    else
        print("Unknown command: ", line);
}

