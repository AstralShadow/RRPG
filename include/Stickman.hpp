#ifndef STICKMAN_HPP
#define STICKMAN_HPP

#include "globals.hpp"
#include "Joint.hpp"
#include <memory>

using std::shared_ptr;

class SDL_Renderer;
class Game;


class Stickman
{
public:
    Stickman(Game*);
    ~Stickman() = default;

    void render(SDL_Renderer*);
    void tick(duration_t);

private:
    Game* _engine;
    Point _pos;
    float _rotation;
    shared_ptr<Joint> _body;
};

#endif
