#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "Scene.hpp"
#include "Actions.hpp"
#include "Texture.hpp"
#include "Sprite.hpp"
#include <string>
#include <stack>
#include <vector>
#include <memory>
#include <chrono>
#include <SDL2/SDL_rect.h>

using std::string;
using std::stack;
using std::vector;
using std::shared_ptr;
using std::chrono::time_point;
using std::chrono::steady_clock;
class SDL_Texture;
class SDL_Surface;
class StoryData;
class Map;


struct ActionPointer
{
    vector<shared_ptr<Action>>* line;
    vector<shared_ptr<Action>>::iterator act;
};

struct EntityState
{
    string name;
    SDL_Point pos;

    Sprite* sprite;
    string state;
    time_point<steady_clock> state_animation_start;
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
    map<string, EntityState> _entities;


    /* Story processing */
    void process_action();
    void increment_action_iterator();
    void process_command(shared_ptr<Command>);

    typedef string Name;
    void set_story(Name);
    void set_map(Name);
    void spawn_entity(Name, SDL_Point, string state);
    void set_entity_state(Name, string state);


    /* Input */
    bool _wait = true;
    bool _dragging = false;

    /* Rendering */
    Map* _map;
    map<char, std::pair<Texture, SDL_Point>> _tilesets;
    
    SDL_Point _screen_size;
    SDL_Point _camera_offset;

    void fit_map_on_screen();
    void render_map(SDL_Renderer*);
    void render_entities(SDL_Renderer*);
};

#endif
