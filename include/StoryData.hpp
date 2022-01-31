#ifndef STORYDATA_HPP
#define STORYDATA_HPP
#include <map>
#include <string>

using std::map;
using std::string;
class Tileset;
class Map;
class Character;
class Story;


struct StoryData
{
    string assets_dir;

    typedef string Name;
    map<Name, Tileset> _tilesets;
    map<Name, Map> _maps;
    map<Name, Character> _characters;
    map<Name, Story> _stories;
};

#endif
