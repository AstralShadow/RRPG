#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "globals.hpp"
#include "Texture.hpp"
#include "StoryData.hpp"
#include <vector>
#include <string>
#include <memory>

using std::string;

class SDL_Window;
class SDL_Renderer;


class Engine
{
public:
    Engine();
    ~Engine();

    int init();
    void load(string assets_dir);

    void run();
    void stop();

    Texture get_texture(string uri);

private:
    bool _running;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    std::vector<Texture> _textures;

    int init_sdl();
    int init_sdl_image();
    int init_window();
    int init_renderer();

    StoryData load_story(string assets_dir);

    Texture load_texture(string uri);
    void clear_textures();

    void poll_events();
    void tick(duration_t);
    void render();

};

#endif
