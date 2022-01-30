#ifndef STR_UTILS_HPP
#define STR_UTILS_HPP

#include <vector>
#include <string>

using std::string;
using std::vector;


string trim(string);
string ltrim(string);
string rtrim(string);

vector<string> explode(char delimeter,
                       string input,
                       int limit = 0);

#endif
