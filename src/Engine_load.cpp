#include "Engine.hpp"
#include "StoryParser.hpp"
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_LOAD_LOG
#include "print.hpp"

using std::string;


void Engine::load(string assets_dir)
{
    print("Parsing story.");
    StoryParser parser(assets_dir);
    parser.parse_file("manifest");
    auto data = parser.get_data();
    print("Caching graphics");
}
