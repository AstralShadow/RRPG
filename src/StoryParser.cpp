#include "StoryParser.hpp"
#include <stdexcept>
#include <fstream>
#include <array>
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"

using std::string;



void StoryParser::parse_file(string uri)
{
    std::ifstream file(uri, std::ios::in);
    std::array<char, 256> buff;
    while(file.getline(&buff[0], 255))
    {
        char* line = &buff[0];
        print(uri, " > ", line);
    }
}
