#ifndef STORYPARSER_HPP
#define STORYPARSER_HPP

#include <string>
using std::string;

class StoryParser
{
public:
    StoryParser() = default;
    ~StoryParser() = default;

    void parse_file(string file);

private:

};

#endif
