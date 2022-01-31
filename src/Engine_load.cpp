#include "Engine.hpp"
#include "StoryParser.hpp"
#include "globals.hpp"
#define ENABLE_PRINTING PRINT_LOAD_LOG
#include "print.hpp"

using std::string;


void Engine::load(string assets_dir)
{
    auto data = load_story(assets_dir);
    data.assets_dir = assets_dir;

#if LOAD_TEXTURES_CACHE
    cache_textures(data);
#endif


}

StoryData Engine::load_story(string assets_dir)
{
    print("Parsing story.");
    StoryParser parser(assets_dir);
    parser.parse_file("manifest");
    return parser.get_data();
}

void Engine::cache_textures(StoryData const& data)
{
    print("Caching textures.");
    for(auto pair : data._tilesets)
        get_texture(pair.second.sprite);
    for(auto pair : data._characters)
        get_texture(pair.second.sprite);
}
