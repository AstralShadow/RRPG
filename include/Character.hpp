#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;


struct Character
{
    Character() = default;
    ~Character() = default;
    
    string name;
    string sprite;
};

#endif
