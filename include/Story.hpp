#ifndef STORY_HPP
#define STORY_HPP

#include "Actions.hpp"
#include "globals.hpp"
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

typedef string StoryName;
typedef string Flag;

struct Story
{
    id_t id;
    StoryName name;
    vector<Action> actions;
    vector<Flag> flags;
    map<Flag, StoryName> events;
};

#endif
