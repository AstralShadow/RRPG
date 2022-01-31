#ifndef PARSERDATA_HPP
#define PARSERDATA_HPP
#include <map>
#include <string>

using std::map;
class Tileset;
class Map;
class Character;
class Story;


struct ParserData
{
    typedef std::string Name;
    map<Name, Tileset> _tilesets;
    map<Name, Map> _maps;
    map<Name, Character> _characters;
    map<Name, Story> _stories;
};

#endif
