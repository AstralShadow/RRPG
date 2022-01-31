#ifndef STORY_HPP
#define STORY_HPP

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
    StoryName name;
    vector<shared_ptr<Action>> actions;
    vector<Flag> flags;
    map<Flag, vector<shared_ptr<Action>>> events;
};

#endif
