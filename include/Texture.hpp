#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

using std::string;
class Game;
class SDL_Texture;
class SDL_Renderer;


class Texture
{
friend Game;

public:
    Texture(const Texture&) = default;
    ~Texture() = default;

    SDL_Texture* get() const { return _data; }
    int w() const { return _w; }
    int h() const { return _h; }

private:
    SDL_Texture* _data = nullptr;
    string _uri;
    int _w = 0;
    int _h = 0;

    Texture() = default;

    int load(SDL_Renderer*, string uri);
    void clear();

};

#endif
