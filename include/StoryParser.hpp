#ifndef STORYPARSER_HPP
#define STORYPARSER_HPP

#include <string>
#include <vector>
#include <map>
#include "Story.hpp"
#include "Character.hpp"

using std::string;
using std::vector;
using std::map;

class StoryParser
{
public:
    StoryParser(string root);

    void parse_file(string);
    void parse_command(string line);
    void parse_state_block(string block);
    void set_speaker(string name);
    void store_text(string text);


private:
    string _root;
    map<string, Character> _characters;
    map<string, Story> _stories;

    string _speaker;
    string _target;

    enum {
        state_none,
        state_character,
        state_story
    } _state;


    void parse_character_command(string& line,
                                 vector<string>& args);
    void parse_story_command(string& line,
                             vector<string>& args);

    void finish_state();

};

#endif
