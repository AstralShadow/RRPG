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
    virtual ~MainMenu();

    void on_enter();
    void tick(duration_t);
    void render(SDL_Renderer*);
    void process(SDL_Event const&);

private:
    milliseconds _ani_progress;
    SDL_Point _screen_size {0, 0};

    Texture _play;
    SDL_Rect _play_area;

    Texture _art;
    SDL_Texture* _art_cache = nullptr;
    SDL_Point _cache_size;
    float _art_offset_x = 0.0f;

    void render_art(SDL_Renderer*);
    void update_art_cache(SDL_Renderer*);
    void render_cache_art(SDL_Renderer*);
};

#endif
