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
    map<Name, Tileset> tilesets;
    map<Name, Map> maps;
    map<Name, Sprite> sprites;
    map<Name, Entity> entities;
    map<Name, Story> stories;
};

#endif
