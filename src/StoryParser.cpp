#include "StoryParser.hpp"
#include "Actions.hpp"
#include "str_utils.hpp"

#include <cstring>
#include <memory>
#include <stdexcept>

#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"


using std::string;
using std::make_shared;
using std::stoi;


StoryParser::StoryParser(string root) :
    _root(root),
    _state(state_none)
{
    print("Assets root: ", root);
}


StoryData StoryParser::get_data()
{
    StoryData data;

    data.tilesets = _tilesets;
    data.maps = _maps;
    data.sprites = _sprites;
    data.entities = _entities;
    data.stories = _stories;

    return data;
}

void StoryParser::parse_command(string line)
{
    auto args = explode(' ', line);
    
    if(args[0] == "include")
    {
        parse_file(args[1]);
        return;
    }
    
    switch(_state)
    {
        case state_sprite:
            parse_sprite_command(line, args);
            break;

        case state_entity:
            parse_entity_command(line, args);
            break;

        case state_tileset:
            parse_tileset_command(line, args);
            break;

        case state_map:
            parse_map_command(line, args);
            break;

        case state_story:
            parse_story_command(line, args);
            break;
        
        default:
            print("Unknown command: ", line);
            break;
    }
}

void StoryParser::parse_state_block(string block)
{
    _state = state_none;
    auto args = explode(' ', block);

    if(_context_stack.size() > 1)
    {
        print("Current line: #", block);
        throw std::runtime_error
            ("Can not change state with" 
            " non-empty context stack "
            "(aka finish your logic blocks and"
            "then do other stories/events).");
    }

    if(args[0] == "sprite")
        begin_sprite_state(block, args);
    else if(args[0] == "entity")
        begin_entity_state(block, args);
    else if(args[0] == "tileset")
        begin_tileset_state(block, args);
    else if(args[0] == "map")
        begin_map_state(block, args);
    else if(args[0] == "story")
        begin_story_state(block, args);
    else if(args[0] == "when")
        begin_event_state(block, args);
    else
        print("Unknown state block #", block);
}
