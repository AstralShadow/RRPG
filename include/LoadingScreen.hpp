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
    LoadingScreen(Engine*, string assets_dir);
    virtual ~LoadingScreen() = default;

    void on_enter();
    void tick(duration_t);
    void render(SDL_Renderer*);
    void process(SDL_Event const&);

private:
    string _root;
    bool _loaded = false;
    #if BUILD_LEVEL_EDITOR
        bool _map_editor = false;
    #endif

    SDL_Rect _pos;
    const char* _message = "Loading...";
    SDL_Color _color{255, 255, 255, 255};

};

#endif
