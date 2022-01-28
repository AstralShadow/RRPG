#ifndef GAME_HPP
#define GAME_HPP

#include "globals.hpp"

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

private:
    bool _running;
    SDL_Window* _window;
    SDL_Renderer* _renderer;

    int init_sdl();
    int init_sdl_image();
    int init_window();
    int init_renderer();

    void poll_events();
    void tick(duration_t);
    void render();

};

#endif
