#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "globals.hpp"
#include "Texture.hpp"
#include "StoryData.hpp"
#include "Scene.hpp"
#include <vector>
#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

class SDL_Window;
class SDL_Renderer;

enum struct EngineMode
{
    loading,
    menu,
    playing,
    creadits
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

    Texture get_texture(string uri);

private:
    bool _running;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    std::vector<Texture> _textures;
    StoryData _story;

    std::map<EngineMode, shared_ptr<Scene>> _scenes;
    EngineMode _mode = EngineMode::loading;


    int init_sdl();
    int init_sdl_image();
    int init_window();
    int init_renderer();

    StoryData load_story(string assets_dir);
    void cache_textures();

    Texture load_texture(string uri);
    void clear_textures();

    void poll_events();
    void tick(duration_t);
    void render();

};

#endif
