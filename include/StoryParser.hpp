#ifndef STORYPARSER_HPP
#define STORYPARSER_HPP

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class Story;
class Character;

class StoryParser
{
public:
    StoryParser(string root);
    ~StoryParser();

    void parse_file(string, bool base = true);
    void parse_command(string line);
    void parse_character_command(string& line,
                                 vector<string>& args);
    void parse_state_block(string block);
    void set_speaker(string name);
    void store_text(string text);


private:
    string _root;
    map<string, Character> _characters;
    map<string, Story> _stories;

    union {
        Character* character;
        Story* story;
    } _target;

    enum {
        state_none,
        state_character,
        state_story
        
    } _state;


    void finish_state();

};

#endif
