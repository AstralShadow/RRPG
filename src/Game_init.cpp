#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


int Game::init()
{
    return init_sdl()
        || init_sdl_image()
        || init_window()
        || init_renderer();
}

int Game::init_sdl()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
        return 0;

    std::cout << "SDL_Init error: "
        << SDL_GetError() << std::endl;
    return 1;
}

int Game::init_sdl_image()
{
    if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
        return 0;

    std::cout << "IMG_Init error: "
        << IMG_GetError() << std::endl;
    return 1;
}

int Game::init_window()
{
    _window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H, 0
    );

    if(_window)
        return 0;

    std::cout << "Could not create window: "
        << SDL_GetError() << std::endl;
    return 1;
}

int Game::init_renderer()
{
    _renderer = SDL_CreateRenderer(
        _window, -1,
        SDL_RENDERER_ACCELERATED
    ); 
    
    if(_renderer)
        return 0;
    
    std::cout << "Could not create renderer: "
        << SDL_GetError() << std::endl;
    return 1;
}

