#ifndef GAME_HPP
#define GAME_HPP

#include "globals.hpp"
#include "Texture.hpp"
#include "Stickman.hpp"
#include <vector>
#include <string>
#include <memory>

class SDL_Window;
class SDL_Renderer;


class Game
{
public:
    Game();
    ~Game();

    int init();

    void run();
    void stop();

    Texture get_texture(std::string uri);

private:
    bool _running;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    std::vector<Texture> _textures;
    Stickman _fighter;

    int init_sdl();
    int init_sdl_image();
    int init_window();
    int init_renderer();

    Texture load_texture(std::string uri);
    void clear_textures();

    void poll_events();
    void tick(duration_t);
    void render();

};

#endif
