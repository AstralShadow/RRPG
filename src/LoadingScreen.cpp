#include "LoadingScreen.hpp"
#include "Engine.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

using std::string;

SDL_Surface* text_surface = nullptr;


LoadingScreen::LoadingScreen(Engine* engine) :
    Scene(engine),
    _pos()
{ }


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
