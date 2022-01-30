#include "str_utils.hpp"
#include <algorithm>

using std::find;


vector<string> explode(char delimeter,
                       string input,
                       int limit)
{
    vector<string> result;

    auto start = input.begin();
    auto itr = start;
    while(itr != input.end()){
        itr = find(start, input.end(), delimeter);
        result.emplace_back(start, itr);
        start = itr + 1;
    } 

    return result;
}
