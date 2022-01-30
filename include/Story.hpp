#ifndef STORY_HPP
#define STORY_HPP

#include "Action.hpp"
#include "globals.hpp"
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

typedef string StoryName;

struct Story
{
    id_t id;
    StoryName name;
};

#endif
