#ifndef LEVEL_EDITOR_HPP
#define LEVEL_EDITOR_HPP

#include "Scene.hpp"
#include <string>

using std::string;


class LevelEditor : public Scene
{
public:
    LevelEditor(Engine* engine);
    virtual ~LevelEditor() = default;

    void on_enter();
    void tick(duration_t);
    void render(SDL_Renderer*);
    void process(SDL_Event const&);

private:

};

#endif
