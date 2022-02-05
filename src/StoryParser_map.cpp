#include "StoryParser.hpp"
#include "str_utils.hpp"
#include <memory>
#include <stdexcept>

#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"

using std::string;
using std::stoi;

void StoryParser::
begin_map_state(string&,
                vector<string>& args)
{
    if(args.size() < 2)
        throw std::runtime_error
            ("Can not load unnamed map.");
    if(args.size() > 2)
        throw std::runtime_error
            ("Map name can not contain spaces.");

    auto name = args[1];

    auto &map = _maps[name];
    map.name = name;

    _target = name;
    _state = state_map;
    print("Loading map ", name);
}

void StoryParser::
parse_map_command(string& line,
                  vector<string>& args)
{
    if(args[0] == "size" && args.size() == 3)
    {
        _maps[_target].size.x = stoi(args[1]);
        _maps[_target].size.y = stoi(args[2]);
        return;
    }

    if(args[0] == "tileset" && args.size() > 1)
    {
        string tileset_name = args[1];
        char key = ' ';
        if(args.size() == 3)
            key = args[2][0];

        _maps[_target].tilesets[key] = tileset_name;
        return;
    }

    print("Unknown command: ", line);
}

void StoryParser::parse_map_data(string line)
{
    auto data = str_split(line);
    for(auto input : data)
    {
        char tileset = ' ';
        if(input[0] < '0' || input[0] > '9')
        {
            tileset = input[0];
            input = input.substr(1);
        }

        if(tileset == '.')
        {
            _maps[_target].data.emplace_back();
            continue;
        }

        auto pos = explode('.', input, 1);
        if(pos.size() == 1)
            _maps[_target].data.emplace_back
                (tileset, stoi(pos[0]));
        else
            _maps[_target].data.emplace_back
                (tileset, stoi(pos[0]), stoi(pos[1]));
    }
}
