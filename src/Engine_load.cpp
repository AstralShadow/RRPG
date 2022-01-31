#include "Engine.hpp"
#include "StoryParser.hpp"
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_LOAD_LOG
#include "print.hpp"

using std::string;


void Engine::load(string assets_dir)
{
    auto data = load_story(assets_dir);
}

StoryData Engine::load_story(string assets_dir)
{
    print("Parsing story.");
    StoryParser parser(assets_dir);
    parser.parse_file("manifest");
    return parser.get_data();
}
