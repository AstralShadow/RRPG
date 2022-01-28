#include "Game.hpp"
#include "Texture.hpp"
#include <string>
#include <algorithm>
#include <stdexcept>


Texture Game::get_texture(std::string uri)
{
    auto texture_itr = 
        std::find_if(_textures.begin(), _textures.end(),
            [uri](Texture texture)
            {
                return texture._uri == uri;
            });

    if(texture_itr == _textures.end())
        return load_texture(uri);

    return *texture_itr;
}

Texture Game::load_texture(std::string uri)
{
    Texture texture;
    int error = texture.load(_renderer, uri);
    if(error)
        throw std::runtime_error
            ("Failed loading resource");

    _textures.push_back(texture);

    return texture;
}

void Game::clear_textures()
{
    for(auto texture : _textures)
        texture.clear();
    _textures.empty();
}
