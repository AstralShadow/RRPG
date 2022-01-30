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


StoryParser::StoryParser(string root) :
    _root(root),
    _state(state_none)
{
    print("Assets root: ", root);
}


void StoryParser::parse_file(string uri)
{
    print("Parsing: ", uri);
    std::ifstream file(_root + uri, std::ios::in);
    std::array<char, 256> buff;
    while(file.getline(&buff[0], 255))
    {
        string line(&buff[0]);
        //print(uri, ": ", line);
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

        if(_state != state_story)
        {
            print("Can not parse outside story mode:");
            print(line);
            return;
        }

        if(line[0] == '>')
        {
            set_speaker(ltrim(line.substr(1)));
            continue;
        }

        store_text(line);
    }

    _state = state_none;
}

void StoryParser::parse_command(string line)
{
    auto args = explode(' ', line);
    if(args[0] == "include")
    {
        parse_file(args[1]);
        return;
    }

    if(_state == state_character)
    {
        parse_character_command(line, args);
        return;
    }

    if(_state == state_story)
    {
        parse_story_command(line, args);
        return;
    }

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
parse_story_command(string& line,
                    vector<string>& args)
{
    if(args[0] == "set" && args.size() == 2)
    {
        auto flag = new SetFlag;
        flag->flag = args[1];
        _stories[_target].actions.emplace_back(flag);
        return;
    }

    print("Unknown command: ", line);
}

void StoryParser::parse_state_block(string block)
{
    _state = state_none;
    auto args = explode(' ', block, 1);

    if(args[0] == "character")
    {
        if(args.size() < 2)
            throw std::runtime_error
                ("Can not load unnamed character");

        auto id = _stories.size();
        string name = args[1];

        auto &character = _characters[name];
        character.name = name;
        character.id = id;
        
        _target = name;
        _state = state_character;
        
        return;
    }

    if(args[0] == "story")
    {
        if(args.size() < 2)
            throw std::runtime_error
                ("Can not load unnamed story.");
        
        string name = args[1];
        auto id = _stories.size();

        auto& story = _stories[name];
        story.id = id;
        story.name = name;

        _target = name;
        _state = state_story;

        return;
    }

    print("Unknown block #", block);
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
    _stories[_target].actions.emplace_back(speech);
}
