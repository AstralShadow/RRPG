#ifndef STORYDATA_HPP
#define STORYDATA_HPP

#include <map>
#include <string>
#include "Tileset.hpp"
#include "Map.hpp"
#include "Character.hpp"
#include "Story.hpp"

using std::map;
using std::string;


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
