#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "Scene.hpp"
#include <string>
#include <SDL2/SDL_rect.h>
#include "Engine.hpp"

using std::string;
class SDL_Texture;
class SDL_Surface;


class MainMenu : public Scene
{
public:
    MainMenu(Engine* e) : Scene(e) {} 
    virtual ~MainMenu() = default;

    void on_enter()
        { _engine->set_mode(EngineMode::playing); }
    void tick(duration_t) { }
    void render(SDL_Renderer*) { }
    void process(SDL_Event const&) { }

private:

};

#endif
