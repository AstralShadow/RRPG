#include "StoryParser.hpp"
#include "str_utils.hpp"
#include <fstream>

#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"

using std::string;


void StoryParser::parse_file(string uri)
{
    #if PRINT_PARSE_READFILE_LOG || 1
        print(" readfile: ", uri);
    #endif

    std::ifstream file(_root + uri, std::ios::in);

    char buffer[256];
    while(file.getline(buffer, 255))
    {
        string line(buffer);

        if(!line.size() || line[0] == ';')
            continue;


        if(line[0] == '#')
            parse_state_block(line.substr(1));
        else if(line[0] == ':')
            parse_command(line.substr(1));
        else if(_state == state_map)
            parse_map_data(line);
        else if(_state == state_story)
            parse_story_data(line);
        else
            print("Can not parse line: \n", line);
    }
}
