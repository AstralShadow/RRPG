#include "Engine.hpp"
#include "StoryParser.hpp"
#include "FontManager.hpp"

#include "globals.hpp"
#define ENABLE_PRINTING PRINT_LOAD_LOG
#include "print.hpp"

#include <chrono>
#include <thread>

using std::string;
using std::chrono::steady_clock;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::duration_cast;


void Engine::load(string assets_dir)
{
    auto start = steady_clock::now();
    
    _story = load_story(assets_dir);
    _story.assets_dir = assets_dir;

    load_fonts();

#if LOAD_TEXTURES_CACHE
    cache_textures();
#endif

    auto end = steady_clock::now();
    auto load_time = duration_cast
        <microseconds>(end - start);
    print("Loaded in ", load_time.count(), "us");

#ifdef DELAY_LOAD_TIME
    auto sleep_time = duration_cast
        <milliseconds>(DELAY_LOAD_TIME - load_time);
    print("Sleeping for ", sleep_time.count(),
          "ms to let you see the loading screen.");
    std::this_thread::sleep_for(DELAY_LOAD_TIME);
#endif
}

StoryData Engine::load_story(string assets_dir)
{
    print("Parsing story.");
    StoryParser parser(assets_dir);
    parser.parse_file("manifest");
    return parser.get_data();
}

void Engine::load_fonts()
{
    print("Loading fonts.");

    int font_sizes[] = PRELOAD_FONT_SIZES;
    for(int size : font_sizes)
        FontManager::get_font
            (DEFAULT_FONT, size);
}

void Engine::cache_textures()
{
    auto root = _story.assets_dir;
    print("Caching textures.");
    for(auto pair : _story.tilesets)
        get_texture(root + pair.second.texture);
    for(auto pair : _story.sprites)
        get_texture(root + pair.second.texture);
}
