#include "Texture.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <iostream>


int Texture::load(SDL_Renderer* renderer, std::string uri)
{
    auto surface = IMG_Load(uri.c_str());
    if(!surface)
    {
        std::cout << "Could not load texture: "
            << uri << std::endl;
        std::cout << "IMG_Load Error: "
            << IMG_GetError() << std::endl;
        return 1;
    }

    store(renderer, surface);
    SDL_FreeSurface(surface);
    return 0;
}

void Texture::store(SDL_Renderer* rnd,
                    SDL_Surface* surface)
{
    _data = SDL_CreateTextureFromSurface(rnd, surface);
    _w = surface->w;
    _h = surface->h;
}

void Texture::clear()
{
    if(_data)
        SDL_DestroyTexture(_data);
    _data = nullptr;
}
