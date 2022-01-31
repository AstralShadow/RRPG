#include "Texture.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <iostream>


int Texture::load(SDL_Renderer* renderer, std::string uri)
{
    auto surf = IMG_Load(uri.c_str());
    if(!surf)
    {
        std::cout << "Could not load texture: "
            << uri << std::endl;
        std::cout << "IMG_Load Error: "
            << IMG_GetError() << std::endl;
        return 1;
    }

    _uri = uri;
    _data = SDL_CreateTextureFromSurface(renderer, surf);
    _w = surf->w;
    _h = surf->h;
    SDL_FreeSurface(surf);

    return 0;
}

void Texture::clear()
{
    if(_data)
        SDL_DestroyTexture(_data);

    _data = nullptr;
    _uri.clear();
}
