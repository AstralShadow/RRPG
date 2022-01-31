#include "LoadingScreen.hpp"
#include <SDL2/SDL_render.h>

using std::string;

LoadingScreen::LoadingScreen(Engine* engine,
                             string root) :
    Scene(engine), _root(root)
{ }

void LoadingScreen::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}
