#include "StoryParser.hpp"
#include <fstream>
#include <cstring>
#include <array>
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"
#include "str_utils.hpp"
#include "Character.hpp"
#include "Story.hpp"
#include <stdexcept>

using std::string;


StoryParser::StoryParser(string root) :
    _root(root),
    _state(state_none)
{
    print("Assets root: ", root);
}

StoryParser::~StoryParser()
{
    finish_state();
}


void StoryParser::parse_file(string uri, bool base)
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
            set_speaker(line.substr(1));
            continue;
        }

        store_text(line);
    }

    if(base)
    {
        finish_state();
    }
}

void StoryParser::parse_command(string line)
{
    auto args = explode(' ', line);
    if(args[0] == "include")
    {
        parse_file(args[1], false);
        return;
    }

    if(_state == state_character){
        parse_character_command(line, args);
        return;
    }

    print("Unknown command: ", line);
}

void StoryParser::
parse_character_command(string& line,
                        vector<string>& args)
{
    if(args[0] == "name")
        _target.character->name = args[1];
    else if(args[0] == "sprite")
        _target.character->sprite = args[1];
    else
        print("Unknown command: ", line);
}

void StoryParser::parse_state_block(string block)
{
    finish_state();
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
        
        _target.character = &character;
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

        _target.story = &story;
        _state = state_story;

        return;
    }

    print("Unknown block #", block);
}

void StoryParser::finish_state()
{
    switch(_state)
    {
        case state_none:
            break;

        case state_character:
        {
            string name = _target.character->name;
            print("Loaded character ", name);
            break;
        }
        
        case state_story:
        {
            string name = _target.story->name;
            print("Loaded story ", name);
            break;
        }
    }

    _state = state_none;
}

void StoryParser::set_speaker(string name)
{
    print("Speaker: ", name);
}

void StoryParser::store_text(string text)
{
    print("Text: ", text);
}
