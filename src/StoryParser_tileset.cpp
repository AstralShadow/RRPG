#include "StoryParser.hpp"
#include "str_utils.hpp"
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"

using std::stoi;


void StoryParser::
begin_tileset_state(string&,
                    vector<string>& args)
{
    if(args.size() < 2)
        throw std::runtime_error
            ("Can not load unnamed tileset.");
    if(args.size() > 2)
        throw std::runtime_error
            ("Tileset name can not contain spaces.");

    auto id = _tilesets.size();
    auto name = args[1];

    auto &tileset = _tilesets[name];
    tileset.id = id;
    tileset.name = name;

    _target = name;
    _state = state_tileset;
    print("Loading tileset ", name);
}

void StoryParser::
parse_tileset_command(string& line,
                      vector<string>& args)
{
    if(args[0] == "file" && args.size() == 2)
        _tilesets[_target].texture = args[1];
    else if(args[0] == "size" && args.size() == 3)
    {
        _tilesets[_target].size.x = stoi(args[1]);
        _tilesets[_target].size.y = stoi(args[2]);
    }
    else
        print("Unknown command: ", line);
}
