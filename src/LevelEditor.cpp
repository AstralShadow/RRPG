#include "LevelEditor.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#include <stdexcept>
#include <utility>
#include <SDL2/SDL_events.h>
#define ENABLE_PRINTING 1
#include "print.hpp"


LevelEditor::LevelEditor(Engine* engine) :
    Scene(engine)
{ }


void LevelEditor::on_enter()
{
    auto& tilesets = _engine->get_story().tilesets;
    if(tilesets.size() == 0)
        throw std::runtime_error
            ("Level editor can't work without tilesets");

    set_tileset(&tilesets.begin()->second);
}

void LevelEditor::tick(duration_t)
{

}

void LevelEditor::process(SDL_Event const& e)
{
    if(e.type == SDL_KEYDOWN)
        process(e.key);

    if(e.type == SDL_MOUSEBUTTONDOWN)
        process(e.button);
    if(e.type == SDL_MOUSEBUTTONUP)
        post_process(e.button);

    if(e.type == SDL_MOUSEMOTION)
        process(e.motion);
}


void LevelEditor::process(SDL_KeyboardEvent const& e)
{
    SDL_Point map_size = _map.size();
    switch(e.keysym.scancode)
    {
        case SDL_SCANCODE_D:
            map_size.x++;
            _map.size(map_size);
            break;
        case SDL_SCANCODE_S:
            map_size.y++;
            _map.size(map_size);
            break;
        case SDL_SCANCODE_A:
            map_size.x--;
            _map.size(map_size);
            break;
        case SDL_SCANCODE_W:
            map_size.y--;
            _map.size(map_size);
            break;

        case SDL_SCANCODE_Q:
            prev_tileset();
            break;
        case SDL_SCANCODE_E:
            next_tileset();
            break;
        case SDL_SCANCODE_LSHIFT:
            _show_menu = !_show_menu;
            break;

        case SDL_SCANCODE_SPACE:
            save();
            break;


        default: break;
    }
}

void LevelEditor::next_tileset()
{
    auto& tilesets = _engine->get_story().tilesets;
    
    auto itr = tilesets.begin();
    while(itr != tilesets.end())
    {
        if(_tileset == &itr->second)
        {
            itr++;
            if(itr != tilesets.end())
            {
                set_tileset(&itr->second);
                return;
            }
            break;
        }
        itr++;
    }

    set_tileset(&tilesets.begin()->second);
}

void LevelEditor::prev_tileset()
{
    auto& tilesets = _engine->get_story().tilesets;
    
    auto prev = tilesets.begin();
    auto itr = prev;
    itr++;
    while(itr != tilesets.end())
    {
        if(_tileset == &itr->second)
        {
            set_tileset(&prev->second);
            return;
        }
        prev = itr++;
    }

    set_tileset(&prev->second);
}

void LevelEditor::set_tileset(Tileset* ptr)
{
    _tileset = ptr;
    print("Tileset: ", ptr->name);

    if(_selection.x + _selection.w > ptr->size.x)
    {
        _selection.x = 0;
        _selection.w = 1;
    }

    if(_selection.y + _selection.h > ptr->size.x)
    {
        _selection.y = 0;
        _selection.h = 1;
    }

    _tileset_offset.x = 0;
    _tileset_offset.y = 0;
}

void LevelEditor::save()
{
    print("Save not implemented");
}


void LevelEditor::process(SDL_MouseButtonEvent const& e)
{
    SDL_Point pos {e.x, e.y};
    _dragging_start = pos;

    if(_show_menu && SDL_PointInRect(&pos, &_menu_area))
    {
        switch(e.button)
        {
            case SDL_BUTTON_LEFT:
                _dragging_mode = D_SELECTION;
                process_selection_dragging(e.x, e.y);
                break;

            case SDL_BUTTON_RIGHT:
                _dragging_mode = D_MOVE_MENU;
                break;

            default: break;
        }
    }
    else
    {
       switch(e.button)
       {
            case SDL_BUTTON_LEFT:
                _dragging_mode = D_MOVE_MAP;
                break;
            
            case SDL_BUTTON_RIGHT:
                _dragging_mode = D_ZOOM_MAP;
                break;

            default: break;
       }
    }
}

void LevelEditor::
post_process(SDL_MouseButtonEvent const& e)
{
    if(!_dragged)
    {
        switch(_dragging_mode)
        {
            case D_ZOOM_MAP:
                _map_zoom = 2.5;
                break;

            case D_MOVE_MAP:
                print("Should've placed some blocks");
                break;

            default: break;
        }
    }
    _dragged = false;
    _dragging_mode = D_NONE;
}

void LevelEditor::process(SDL_MouseMotionEvent const& e)
{
    switch(_dragging_mode)
    {
        case D_SELECTION:
            process_selection_dragging(e.x, e.y);
            break;

        case D_MOVE_MENU:
            _tileset_offset.x += e.xrel;
            _tileset_offset.y += e.yrel;
            break;

        case D_MOVE_MAP:
            _map_offset.x += e.xrel;
            _map_offset.y += e.yrel;
            break;

        case D_ZOOM_MAP:
            _map_zoom += e.xrel / 100.0f;
            break;

        default:
            return;
    }

    _dragged = true;
}

void LevelEditor::
process_selection_dragging(int x, int y)
{
    SDL_Point pos = _dragging_start;
    SDL_Point delta = {x - pos.x, y - pos.y};
    pos.x -= _menu_area.x + _tileset_offset.x;
    pos.y -= _menu_area.y + _tileset_offset.y;
    float tile_size = 32 * 1.5;

    int x1 = pos.x / tile_size;
    int y1 = pos.y / tile_size;
    int x2 = (pos.x + delta.x) / tile_size;
    int y2 = (pos.y + delta.y) / tile_size;

    if(x1 > x2)
        std::swap(x1, x2);
    if(y1 > y2)
        std::swap(y1, y2);

    if(x1 < 0) x1 = 0;
    if(y1 < 0) y1 = 0;

    x2 += 1 - x1;
    y2 += 1 - y1;

    if(x1 > _tileset->size.x - 1)
        x1 = _tileset->size.x - 1;
    if(y1 > _tileset->size.y - 1)
        y1 = _tileset->size.y - 1;
    if(x1 + x2 > _tileset->size.x)
        x2 = _tileset->size.x - x1;
    if(y1 + y2 > _tileset->size.y)
        y2 = _tileset->size.y - y1;

    _selection = {x1, y1, x2, y2};
}


