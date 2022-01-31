#ifndef SCENE_HPP
#define SCENE_HPP

#include "globals.hpp"
class Engine;
class SDL_Renderer;
union SDL_Event;


class Scene
{
public:
    Scene(Engine* engine) :
        _engine(engine) { }
    virtual ~Scene() { };

    virtual void on_enter() = 0;
    virtual void tick(duration_t) = 0;
    virtual void render(SDL_Renderer*) = 0;
    virtual void process(SDL_Event const&) = 0;

protected:
    Engine* _engine;
};


#endif
