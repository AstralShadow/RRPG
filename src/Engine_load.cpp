#include "Engine.hpp"
#include "StoryParser.hpp"
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_LOAD_LOG
#include "print.hpp"

using std::string;


void Engine::load(string assets_dir)
{
    string manifest_uri = assets_dir + "manifest";
    print("Parsing assets manifest file ", manifest_uri);
    StoryParser parser;

    parser.parse_file(manifest_uri);

}
