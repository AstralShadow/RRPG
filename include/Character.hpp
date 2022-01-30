#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "globals.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;


struct Character
{
    Character() = default;
    ~Character() = default;
    
    id_t id;
    string name;
    string sprite;
};

#endif
