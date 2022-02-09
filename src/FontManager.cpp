#include "FontManager.hpp"
#include <SDL2/SDL_ttf.h>
#include <stdexcept>


FontManager::FontCollection FontManager::_fonts = {};


TTF_Font* FontManager::get_font(string uri, int size)
{
    auto key = std::make_pair(uri, size);

    auto itr = _fonts.find(key);
    if(itr != _fonts.end())
        return itr->second;

    TTF_Font* font = TTF_OpenFont(uri.c_str(), size);
    if(!font)
    {
        string msg = "Error opening font. \n";
        msg += TTF_GetError();
        throw std::runtime_error(msg.c_str());
    }

    _fonts[key] = font;
    
    return font;
}

void FontManager::empty_cache()
{
    for(auto pair : _fonts)
        TTF_CloseFont(pair.second);
    _fonts.clear();
}
