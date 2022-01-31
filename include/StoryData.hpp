#ifndef STORYDATA_HPP
#define STORYDATA_HPP

#include <map>
#include <string>
#include "Tileset.hpp"
#include "Map.hpp"
#include "Sprite.hpp"
#include "Entity.hpp"
#include "Story.hpp"

using std::map;
using std::string;


struct StoryData
{
    string assets_dir;

    typedef string Name;
    map<Name, Tileset> _tilesets;
    map<Name, Map> _maps;
    map<Name, Sprite> _sprites;
    map<Name, Entity> _entities;
    map<Name, Story> _stories;
};

#endif
