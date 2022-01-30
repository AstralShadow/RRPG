#include "StoryParser.hpp"
#include <fstream>
#include <cstring>
#include <array>
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"
#include "str_utils.hpp"
#include <stdexcept>

using std::string;


StoryParser::StoryParser(string root) :
    _root(root),
    _state(state_none)
{ }

StoryParser::~StoryParser()
{
    finish_state();
}


void StoryParser::parse_file(string uri, bool base)
{
    print("Parsing asset file: ", uri);
    std::ifstream file(_root + uri, std::ios::in);
    std::array<char, 256> buff;
    while(file.getline(&buff[0], 255))
    {
        string line(&buff[0]);
        //print(uri, ": ", line);
        line = trim(line);

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
    if(block == "character")
    {
        finish_state();
        _state = state_character;
        _target.character = new Character();
        return;
    }

    auto args = explode(' ', block);
    if(args[0] == "story")
    {
        
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
            print("Loaded character '",
                    _target.character->name, '\'');

            _characters.push_back(*_target.character);
            delete _target.character;
            break;
        }
        
        case state_story:
        {
            if(_target.story->name == "")
            {
                print("Deleting unnamed story block.");
                delete _target.story;
                break;
            }
            
            print("Loaded story ", _target.story->name);
            _target.story = nullptr;

            break;
        }
    }

    _state = state_none;
}

void StoryParser::set_speaker(string name)
{

}

void StoryParser::store_text(string text)
{

}
