#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

using std::string;
class Engine;
class SDL_Texture;
class SDL_Surface;
class SDL_Renderer;


class Texture
{
friend Engine;

public:
    Texture() = default;
    Texture(const Texture&) = default;
    ~Texture() = default;

    SDL_Texture* get() const { return _data; }
    operator SDL_Texture*() const { return _data; }
    int w() const { return _w; }
    int h() const { return _h; }

private:
    SDL_Texture* _data = nullptr;
    int _w = 0;
    int _h = 0;


    int load(SDL_Renderer*, string uri);
    void store(SDL_Renderer*, SDL_Surface*);
    void clear();

};

#endif
