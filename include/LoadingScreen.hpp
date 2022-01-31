#ifndef LOADING_SCREEN_HPP
#define LOADING_SCREEN_HPP

#include "Scene.hpp"
#include <string>
#include <SDL2/SDL_rect.h>

using std::string;
class SDL_Texture;
class SDL_Surface;


class LoadingScreen : public Scene
{
public:
    LoadingScreen(Engine*, string root);
    virtual ~LoadingScreen();

    void on_enter() { }
    void tick(duration_t) { }
    void render(SDL_Renderer*);
    void process(SDL_Event const&) { }

private:
    SDL_Surface* _logo_raw;
    SDL_Texture* _logo;
    SDL_Rect _pos;

};

#endif
