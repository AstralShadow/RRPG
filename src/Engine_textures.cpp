#include "Engine.hpp"
#include "Texture.hpp"
#include "FontManager.hpp"
#include <string>
#include <stdexcept>
#include <SDL2/SDL_ttf.h>

using std::string;


Texture Engine::get_texture(string uri)
{
    auto itr = _textures.find(uri);
    if(itr != _textures.end())
        return itr->second;

    return load_texture(uri);
}

Texture Engine::load_texture(std::string uri)
{
    auto& texture = _textures[uri];
    int error = texture.load(_renderer, uri);
    if(error)
    {
        throw std::runtime_error
            ("Failed loading resource");
    }

    return texture;
}

Texture Engine::get_text(string text,
                         SDL_Color color,
                         int size)
{
    auto key = std::make_tuple(text, color, size);
    auto itr = _texts.find(key);
    if(itr != _texts.end())
        return itr->second;
    
    return render_text(text, color, size);
}

Texture Engine::render_text(string text,
                            SDL_Color color,
                            int size)
{
    auto key = std::make_tuple(text, color, size);
    auto font = FontManager::get_font(DEFAULT_FONT, size);
    auto& texture = _texts[key];

    auto tmp = TTF_RenderText_Blended
        (font, text.c_str(), color);
    texture.store(_renderer, tmp);
    SDL_FreeSurface(tmp);

    return texture;
}

void Engine::clear_textures()
{
    for(auto pair : _textures)
        pair.second.clear();
    _textures.clear();

    for(auto pair : _texts)
        pair.second.clear();
    _texts.clear();
}
