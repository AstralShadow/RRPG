#ifndef LOADING_SCREEN_HPP
#define LOADING_SCREEN_HPP

#include "Scene.hpp"
#include <string>

using std::string;


class LoadingScreen : public Scene
{
public:
    LoadingScreen(Engine*, string root);
    virtual ~LoadingScreen() = default;

    void on_enter() { }
    void tick(duration_t) { }
    void render(SDL_Renderer*);
    void process(SDL_Event const&) { }

private:
    string _root;
};

#endif
