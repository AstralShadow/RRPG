#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "globals.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;


struct Entity
{
    id_t id;
    string name;
    string sprite;
};

#endif
