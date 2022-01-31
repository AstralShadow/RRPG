#ifndef STORYDATA_HPP
#define STORYDATA_HPP
#include <map>
#include <string>

using std::map;
class Tileset;
class Map;
class Character;
class Story;


struct StoryData
{
    typedef std::string Name;
    map<Name, Tileset> _tilesets;
    map<Name, Map> _maps;
    map<Name, Character> _characters;
    map<Name, Story> _stories;
};

#endif
