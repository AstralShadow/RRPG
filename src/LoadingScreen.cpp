#include "LoadingScreen.hpp"
#include "FontManager.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

using std::string;

SDL_Surface* text_surface = nullptr;


LoadingScreen::LoadingScreen(Engine* engine,
                             string root) :
    Scene(engine),
    _logo_raw(nullptr),
    _logo(nullptr),
    _pos()
{
    string uri = root + "fonts/" + FONT_NAME;
    auto font = FontManager::get_font(uri, 32);

    const char* msg = "Loading...";
    SDL_Color color{255, 255, 255, 255};

    _logo_raw = TTF_RenderText_Blended(font, msg, color);
}

LoadingScreen::~LoadingScreen()
{
    if(_logo_raw)
        SDL_FreeSurface(_logo_raw);
    if(_logo)
        SDL_DestroyTexture(_logo);
}


void LoadingScreen::render(SDL_Renderer* renderer)
{
    if(_logo == nullptr && _logo_raw != nullptr)
    {
        _logo = SDL_CreateTextureFromSurface
            (renderer, _logo_raw);
        SDL_FreeSurface(_logo_raw);
        _logo_raw = nullptr;
        SDL_QueryTexture(_logo, nullptr, nullptr,
                                &_pos.w, &_pos.h);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    SDL_GetRendererOutputSize(renderer, &_pos.x, &_pos.y);
    _pos.x = (_pos.x - _pos.w) / 2;
    _pos.y = (_pos.y - _pos.h) / 2;

    SDL_RenderCopy(renderer, _logo, nullptr, &_pos);
}
