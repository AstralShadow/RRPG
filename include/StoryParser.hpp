#ifndef STORYPARSER_HPP
#define STORYPARSER_HPP

#include <string>
#include <vector>
#include <map>
#include <stack>
#include "Story.hpp"
#include "Character.hpp"
#include "Tileset.hpp"

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::stack;

typedef string Flag;
typedef string StoryName;


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
    map<string, Tileset> _tilesets;
    map<StoryName, Story> _stories;

    map<pair<StoryName, Flag>, vector<shared_ptr<Action>>>
        _unlinked_events;
    string _speaker;
    string _target;
    typedef vector<shared_ptr<Action>> ActionList;
    stack<ActionList*> _context_stack;

    enum {
        state_none,
        state_character,
        state_tileset,
        state_story
    } _state;


    void parse_character_command(string& line,
                                 vector<string>& args);
    void parse_tileset_command(string& line,
                               vector<string>& args);
    void parse_story_command(string& line,
                             vector<string>& args);

    void finish_state();

};

#endif
