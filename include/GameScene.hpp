#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "Scene.hpp"
#include "Actions.hpp"
#include "Texture.hpp"
#include "Sprite.hpp"
#include "Point.hpp"
#include "Direction.hpp"
#include <string>
#include <stack>
#include <memory>
#include <chrono>
#include <set>
#include <map>
#include <vector>
#include <forward_list>
#include <SDL2/SDL_rect.h>

using std::string;
using std::stack;
using std::vector;
using std::shared_ptr;
using std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::milliseconds;
using std::forward_list;
using std::set;
class SDL_Texture;
class SDL_Surface;
class SDL_MouseButtonEvent;
class StoryData;
class Map;


typedef vector<shared_ptr<Action>> StoryArc;

struct ActionPointer
{
    StoryArc* line;
    StoryArc::iterator act;
};

struct EntityState
{
    string name;
    Point pos;

    Sprite* sprite;
    Texture texture;
    string state;
    duration_t animation_progress;
    Direction direction;
};

struct Motion
{
    string entity;
    duration_t progress;
    duration_t length;
    Point start;
    Point end;
};

struct SpeechBubble
{
    string entity;
    Texture text;

    Point pos;
    duration_t age;

    
    typedef vector<std::pair<Texture,
                             StoryArc*>> ChoiceOptions;

    shared_ptr<ChoiceOptions> options = nullptr;

    float bottom_margin = 0;
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
    void make_choice(SDL_MouseButtonEvent const&);

private:
    StoryData* _data;

    /* Progress context */
    string _story;
    stack<ActionPointer> _linestack;
    map<string, EntityState> _entities;
    set<Flag> _flags;
    vector<SpeechBubble> _speeches;


    /* Story processing */
    void process_action();
    void increment_action_iterator();
    void process_command(shared_ptr<Command>);
    void set_flag(shared_ptr<SetFlag>);
    void process_condition(shared_ptr<Condition>);
    void create_speech(shared_ptr<Speech>);
    void create_choice(shared_ptr<Choice>);

    typedef string Name;
    void set_story(Name);
    void run_story_arc(StoryArc*);
    void set_map(Name);
    void spawn_entity(Name, SDL_Point, string state);
    void set_entity_state(Name, string state);
    void set_entity_direction(Name, string direction);
    void move_entity(Name, SDL_Point);
    void remove_entity(Name);
    void move_camera(SDL_Point);

    /* Input */
    bool _wait_input = true;
    duration_t _time_to_wait;
    bool _dragging = false;
    Point _drag_start{0, 0};
    float _zoom = 2.5;

    /* Ticking */
    forward_list<Motion> _motions;

    void update_animations(duration_t);
    void update_motions(duration_t);
    void remove_old_speeches(duration_t);
    void position_speeches();
    void position_speeches_horizontally();
    void position_speeches_vertically();

    void sleep(milliseconds);
    void wait_entity_motion(Name);
    void finish_motion(Motion& motion);

    /* Rendering */
    Map* _map;
    map<char, std::pair<Texture, SDL_Point>> _tilesets;
    
    SDL_Point _screen_size{SCREEN_W, SCREEN_H};
    Point _camera_offset;
    bool _show_grid_locations = false;

    void fit_map_on_screen();
    void render_map(SDL_Renderer*);
    void render_entities(SDL_Renderer*);
    void render_speeches(SDL_Renderer*);

    void render_speech_bubble(SDL_Renderer*,
                              Texture& texture,
                              SDL_Rect area,
                              Point* entity_pos);
    void render_speech_bubble_edges
        (SDL_Renderer*, Texture&, SDL_Rect area);
    void render_speech_bubble_vertical_borders
        (SDL_Renderer*, Texture&, SDL_Rect area);
    void render_speech_bubble_horizontal_borders
        (SDL_Renderer*, Texture&, SDL_Rect, int ptr_pos);
    void render_speech_bubble_background
        (SDL_Renderer*, Texture&, SDL_Rect, int ptr_pos);
    void render_speech_bubble_horizontal_borders
        (SDL_Renderer*, Texture&, SDL_Rect);
    void render_speech_bubble_background
        (SDL_Renderer*, Texture&, SDL_Rect);
};

#endif
