#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "Scene.hpp"
#include "Texture.hpp"
#include <string>
#include <chrono>
#include <SDL2/SDL_rect.h>

using std::string;
using std::chrono::milliseconds;
class SDL_Texture;
class SDL_Surface;
class Engine;


class MainMenu : public Scene
{
    static const int _play_motion_height = 16;

public:
    MainMenu(Engine* e);
    virtual ~MainMenu() = default;

    void on_enter();
    void tick(duration_t);
    void render(SDL_Renderer*);
    void process(SDL_Event const&);

private:
    Texture _art;
    Texture _play;
    milliseconds _ani_progress;

    SDL_Point _screen_size {800, 600};
    SDL_Rect _play_area;
    int _art_offset_x = 0;

    void render_art(SDL_Renderer*);
};

#endif
