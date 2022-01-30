#include "str_utils.hpp"
#include <algorithm>

using std::find;


string trim(string input)
{
    return rtrim(ltrim(input));
}

string ltrim(string input)
{
    auto begin = input.begin();
    auto end = input.end();
    while(*begin == ' ' || *begin == '\t') begin++;
    return string(begin, end);
}

string rtrim(string input)
{
    auto begin = input.begin();
    auto end = input.end();
    while(*(end-1) == ' ' || *(end-1) == '\t') end--;
    return string(begin, end);
}

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
        limit--;
        if(limit == 0) break;
    }

    if(itr != input.end())
        result.emplace_back(start, input.end());

    return result;
}
