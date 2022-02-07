#include "LoadingScreen.hpp"
#include "Engine.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

using std::string;

SDL_Surface* text_surface = nullptr;


LoadingScreen::LoadingScreen(Engine* engine,
                             string root) :
    Scene(engine),
    _root(root),
    _pos()
{ }

void LoadingScreen::on_enter()
{
    _engine->update_screen();
    _engine->load(_root);
    _loaded = true;
}

void LoadingScreen::tick(duration_t)
{
    if(!_loaded)
        return;
    
#if BUILD_LEVEL_EDITOR
    #if BUILD_LEVEL_EDITOR == 3
        _map_editor = true;
    #endif
    if(_map_editor)
        _engine->set_mode(EngineMode::map_editor);
    else
        _engine->set_mode(EngineMode::menu);
#else
    _engine->set_mode(EngineMode::menu);
#endif
}

void LoadingScreen::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    auto logo = _engine->get_text(_message, _color, 32);
    _pos.w = logo.w();
    _pos.h = logo.h();
    
    SDL_GetRendererOutputSize(renderer, &_pos.x, &_pos.y);
    _pos.x = (_pos.x - _pos.w) / 2;
    _pos.y = (_pos.y - _pos.h) / 2;

    SDL_RenderCopy(renderer, logo.get(), nullptr, &_pos);
}

#if BUILD_LEVEL_EDITOR
void LoadingScreen::process(SDL_Event const& e)
{
    if(e.type != SDL_KEYDOWN)
        return;
    
    if(e.key.keysym.scancode == SDL_SCANCODE_M)
        _map_editor = true;
}
#else
void LoadingScreen::process(SDL_Event const&)
{ }
#endif
