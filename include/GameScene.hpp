#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "Scene.hpp"
#include "Actions.hpp"
#include <string>
#include <stack>
#include <vector>
#include <memory>
#include <SDL2/SDL_rect.h>

using std::string;
using std::stack;
using std::vector;
using std::shared_ptr;
class SDL_Texture;
class SDL_Surface;
class StoryData;


struct ActionPointer
{
    vector<shared_ptr<Action>>* line;
    vector<shared_ptr<Action>>::iterator act;
};

class GameScene : public Scene
{
public:
    GameScene(Engine*, string);
    virtual ~GameScene();

    void on_enter();
    void tick(duration_t);
    void render(SDL_Renderer*);
    void process(SDL_Event const&);

private:
    StoryData* _data;

    /* Progress context */
    string _story;
    stack<ActionPointer> _linestack;

    void process_action();
    void increment_action_iterator();

};

#endif
