#ifndef LEVEL_EDITOR_HPP
#define LEVEL_EDITOR_HPP

#include "Scene.hpp"
#include <string>
#include <vector>
#include <SDL2//SDL_rect.h>
class Tileset;
struct SDL_KeyboardEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseMotionEvent;

using std::string;
using std::vector;


class LevelEditor : public Scene
{
public:
    class Map
    {
    public:
        struct TileData
        {
            bool empty = true;
            string tileset;
            int x;
            int y;
        };

        Map();

        SDL_Point size() { return _size; }
        void size(SDL_Point);

        TileData& at(int x, int y);
        TileData& operator [](int);

    private:
        SDL_Point _actual_size {10, 10};
        SDL_Point _size {10, 10};
        TileData* _data = nullptr;

        void resize(SDL_Point);
    };

    LevelEditor(Engine* engine);
    virtual ~LevelEditor() = default;

    void on_enter();
    void tick(duration_t);
    void render(SDL_Renderer*);
    void process(SDL_Event const&);
    void process(SDL_KeyboardEvent const&);
    void process(SDL_MouseButtonEvent const&);
    void process(SDL_MouseMotionEvent const&);

private:
    bool _show_menu = true;

    SDL_Point _screen;
    
    SDL_Rect _map_area{0, 0, -1, -1};
    SDL_Rect _menu_area{-1, -1, 240, 240};

    Map _map;
    SDL_Point _map_offset{0, 0};
    
    string _tileset_name;
    Tileset* _tileset = nullptr;
    SDL_Point _tileset_offset{0, 0};
    SDL_Rect _selection{0, 0, 1, 1};

    enum {
        D_NONE,
        D_SELECTION,
        D_MOVE_MENU,
        D_MOVE_MAP
    } _dragging_mode;
    SDL_Point _dragging_start{0, 0};


    void process_selection_dragging(int x, int y);


    void render_map(SDL_Renderer*);
    void render_menu(SDL_Renderer*);

    void prev_tileset();
    void next_tileset();
    void set_tileset(Tileset*);
    void save();

};


#endif
