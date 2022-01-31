#ifndef STORYPARSER_HPP
#define STORYPARSER_HPP

#include "StoryData.hpp"
#include <string>
#include <vector>
#include <map>
#include <stack>

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

    StoryData get_data();

private:
    string _root;
    map<string, Sprite> _sprites;
    map<string, Entity> _entities;
    map<string, Tileset> _tilesets;
    map<string, Map> _maps;
    map<StoryName, Story> _stories;

    map<pair<StoryName, Flag>, vector<shared_ptr<Action>>>
        _unlinked_events;
    string _speaker;
    string _target;
    typedef vector<shared_ptr<Action>> ActionList;
    stack<ActionList*> _context_stack;

    enum {
        state_none,
        state_sprite,
        state_entity,
        state_tileset,
        state_map,
        state_story
    } _state;

    
    /* Sprite */
    void begin_sprite_state(string& line,
                            vector<string>& args);
    void parse_sprite_command(string& line,
                              vector<string>& args);

    /* Entity */
    void begin_entity_state(string& line,
                            vector<string>& args);
    void parse_entity_command(string& line,
                              vector<string>& args);

    /* Tileset */
    void begin_tileset_state(string& line,
                             vector<string>& args);
    void parse_tileset_command(string& line,
                               vector<string>& args);
    /* Map */
    void begin_map_state(string& line,
                         vector<string>& args);
    void parse_map_command(string& line,
                           vector<string>& args);
    void parse_map_data(string line);

    /* Story */
    void begin_story_state(string& line,
                           vector<string>& args);
    void begin_event_state(string& line,
                           vector<string>& args);
    void parse_story_data(string line);
    void parse_story_command(string& line,
                             vector<string>& args);
    void set_speaker(string name);
    void store_text(string text);
};

#endif
