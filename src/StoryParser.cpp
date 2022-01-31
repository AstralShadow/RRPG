#include "StoryParser.hpp"
#include <fstream>
#include <cstring>
#include <array>
#include <memory>
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"
#include "str_utils.hpp"
#include "Character.hpp"
#include "Story.hpp"
#include "Actions.hpp"
#include <stdexcept>

using std::string;
using std::make_shared;
using std::stoi;


StoryParser::StoryParser(string root) :
    _root(root),
    _state(state_none)
{
    print("Assets root: ", root);
}


ParserData StoryParser::get_data()
{
    ParserData data;

    data._tilesets = _tilesets;
    data._maps = _maps;
    data._characters = _characters;
    data._stories = _stories;

    return data;
}


void StoryParser::parse_file(string uri)
{
#if PRINT_PARSE_READFILE_LOG
    print(" readfile: ", uri);
#endif
    std::ifstream file(_root + uri, std::ios::in);
    std::array<char, 256> buff;
    while(file.getline(&buff[0], 255))
    {
        string line(&buff[0]);
        line = trim(line);

        if(!line.size())
            continue;

        if(line[0] == ';')
            continue;

        if(line[0] == '#')
        {
            parse_state_block(line.substr(1));
            continue;
        }
        if(line[0] == ':')
        {
            parse_command(line.substr(1));
            continue;
        }

        if(_state == state_story)
        {
            if(line[0] == '>')
            {
                set_speaker(ltrim(line.substr(1)));
                continue;
            }

            store_text(line);
            continue;
        }

        if(_state == state_map)
        {
            store_map_data(line);
            continue;
        }

        print("Can not parse line in current mode:");
        print(line);
    }
}

void StoryParser::parse_command(string line)
{
    auto args = explode(' ', line);
    
    if(args[0] == "include")
        parse_file(args[1]);
    else if(_state == state_character)
        parse_character_command(line, args);
    else if(_state == state_tileset)
        parse_tileset_command(line, args);
    else if(_state == state_story)
        parse_story_command(line, args);
    else if(_state == state_map)
        parse_map_command(line, args);
    else
        print("Unknown command: ", line);
}

void StoryParser::
parse_character_command(string& line,
                        vector<string>& args)
{
    if(args[0] == "sprite")
        _characters[_target].sprite = args[1];
    else
        print("Unknown command: ", line);
}

void StoryParser::
parse_tileset_command(string& line,
                      vector<string>& args)
{
    if(args[0] == "sprite" && args.size() == 2)
        _tilesets[_target].sprite = args[1];
    else if(args[0] == "size" && args.size() == 3)
    {
        _tilesets[_target].size.x = stoi(args[1]);
        _tilesets[_target].size.y = stoi(args[2]);
    }
    else
        print("Unknown command: ", line);
}

void StoryParser::
parse_story_command(string& line,
                    vector<string>& args)
{
    if(args[0] == "set" && args.size() == 2)
    {
        auto flag = new SetFlag;
        flag->flag = args[1];
        _context_stack.top()->emplace_back(flag);
        return;
    }

    if(args[0] == "move" && args.size() == 3)
    {
        auto cmd = new Command;
        cmd->command = Command::move;
        cmd->name = _speaker;
        cmd->pos.x = stoi(args[1]);
        cmd->pos.y = stoi(args[2]);
        _context_stack.top()->emplace_back(cmd);
        return;
    }

    if(args[0] == "choice" && args.size() > 2)
    {
        auto name = explode(' ', line, 1)[1];
        auto& action_list = _context_stack.top();
        auto last_action = action_list->back();
        if(last_action->type() == Action::action_choice)
        {
            auto last = std::static_pointer_cast
                <Choice>(last_action);
            if(last->character == _speaker)
            {
                _context_stack.push
                    (&(last->options[name]));
                return;
            }
        }
        
        auto choice = new Choice;
        choice->character = _speaker;

        _context_stack.top()->emplace_back(choice);
        _context_stack.push(&(choice->options[name]));
        return;
    }

    if(args[0] == "if" && args.size() == 2)
    {
        Flag flag(args[1]);
        auto block = new Condition;
        block->flag = flag;
        
        _context_stack.top()->emplace_back(block);
        _context_stack.push(&block->actions);
        return;
    }

    if(args[0] == "else" && args.size() == 1)
    {
        auto prev_list = _context_stack.top();
        _context_stack.pop();
        auto last_action = _context_stack.top()->back();
        auto last_type = last_action->type();
        if(last_type != Action::action_condition)
            throw std::runtime_error
                ("You can call :else only after :if");
        
        auto condition = std::static_pointer_cast
            <Condition>(last_action);
        if(&(condition->alternative) == prev_list)
            throw std::runtime_error
                ("Only one else block per :if allowed");

        _context_stack.push(&(condition->alternative));
        return;
    }

    if(args[0] == "end" && args.size() == 1)
    {
        _context_stack.pop();
        if(_context_stack.size() == 0)
            _state = state_none;
        return;
    }

    print("Unknown command: ", line);
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

void StoryParser::parse_state_block(string block)
{
    _state = state_none;
    auto args = explode(' ', block);

    if(_context_stack.size() > 1)
        throw std::runtime_error
            ("Can not change state with" 
            " non-empty context stack "
            "(aka finish your logic blocks and"
            "then do other stories/events).");

    if(args[0] == "character")
    {
        if(args.size() < 2)
            throw std::runtime_error
                ("Can not load unnamed character");

        auto id = _stories.size();
        string name = explode(' ', block, 1)[1];

        auto &character = _characters[name];
        character.name = name;
        character.id = id;
        
        _target = name;
        _state = state_character;
        print("Loading character ", name);
        
        return;
    }

    if(args[0] == "tileset")
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

        return;
    }

    if(args[0] == "map")
    {
        if(args.size() < 2)
            throw std::runtime_error
                ("Can not load unnamed map.");
        if(args.size() > 2)
            throw std::runtime_error
                ("Map name can not contain spaces.");

        auto id = _maps.size();
        auto name = args[1];

        auto &map = _maps[name];
        map.id = id;
        map.name = name;

        _target = name;
        _state = state_map;
        print("Loading map ", name);

        return;
    }

    if(args[0] == "story")
    {
        if(args.size() < 2)
            throw std::runtime_error
                ("Can not load unnamed story.");
        if(args.size() > 2)
            throw std::runtime_error
                ("Story name can not contain spaces.");
        
        string name = args[1];
        auto id = _stories.size();

        auto& story = _stories[name];
        story.id = id;
        story.name = name;

        _target = name;
        _context_stack.empty();
        _context_stack.push(&(story.actions));
        _state = state_story;
        print("Loading story ", name);

        return;
    }

    if(args[0] == "when")
    {
        if(args.size() != 3)
            throw std::runtime_error
                ("Format: #when <storyname> <flag>");

        auto story= args[1];
        auto flag = args[2];
        
        _target = story;
        _context_stack.empty();
        _context_stack
            .push(&(_stories[story].events[flag]));
        _state = state_story;
        print("Loading event ", flag, " in ", story);

        return;
    }

    print("Unknown state block #", block);
}

void StoryParser::set_speaker(string name)
{
    if(name != "")
    {
        auto entity = _characters.find(name);
        if(entity == _characters.end())
            print("WARNING: Character ", name,
                  " is not loaded, but used in story ",
                  _stories[_target].name);
    }
    _speaker = name;
}

void StoryParser::store_text(string text)
{
    auto speech = new Speech;
    speech->character = _speaker;
    speech->text = text;
    _context_stack.top()->emplace_back(speech);
}

void StoryParser::store_map_data(string line)
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

        auto pos = explode('.', input, 1);
        if(pos.size() == 1)
            _maps[_target].data.emplace_back
                (tileset, stoi(pos[0]));
        else
            _maps[_target].data.emplace_back
                (tileset, stoi(pos[0]), stoi(pos[1]));
    }
}
