#ifndef STORY_HPP
#define STORY_HPP

#include "globals.hpp"
#include <string>
#include <vector>
#include <map>
#include <memory>

using std::string;
using std::vector;
using std::map;
using std::shared_ptr;

typedef string StoryName;
typedef string Flag;
class Action;

struct Story
{
    id_t id;
    StoryName name;
    vector<shared_ptr<Action>> actions;
    vector<Flag> flags;
    map<Flag, StoryName> events;
};

#endif
