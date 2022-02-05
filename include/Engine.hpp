#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "globals.hpp"
#include "Texture.hpp"
#include "StoryData.hpp"
#include "Scene.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <tuple>
#include <SDL2/SDL_pixels.h>

using std::string;
using std::shared_ptr;
using std::map;

class SDL_Window;
class SDL_Renderer;


enum struct EngineMode
{
    loading,
    menu,
    playing,
    credits,
    map_editor
};


class Engine
{
public:
    Engine();
    ~Engine();

    int init();
    void load(string assets_dir);
    void set_scene(EngineMode, shared_ptr<Scene>);
    void set_mode(EngineMode);
    StoryData& get_story() { return _story; }

    void run();
    void stop();
    void update_screen() { render(); }

    Texture get_texture(string uri);
    Texture get_text(string, SDL_Color, int size = 18);

private:
    bool _running;
    SDL_Window* _window;
    SDL_Renderer* _renderer;

    typedef std::map<string, Texture> TextureCache;
    typedef std::map<std::tuple<string, SDL_Color, int>,
                     Texture> TextCache;
    TextureCache  _textures;
    TextCache _texts;
    StoryData _story;

    std::map<EngineMode, shared_ptr<Scene>> _scenes;
    EngineMode _mode = EngineMode::loading;


    int init_sdl();
    int init_sdl_image();
    int init_sdl_ttf();
    int init_window();
    int init_renderer();

    StoryData load_story(string assets_dir);
    void load_fonts();
    void cache_textures();

    Texture load_texture(string uri);
    Texture render_text(string, SDL_Color, int size = 14);
    void clear_textures();

    void poll_events();
    void tick(duration_t);
    void render();

};

#endif
