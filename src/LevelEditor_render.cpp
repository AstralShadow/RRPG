#include "LevelEditor.hpp"
#include "Engine.hpp"
#include <SDL2/SDL_render.h>


void LevelEditor::render(SDL_Renderer* rnd)
{
    SDL_GetRendererOutputSize(rnd, &_screen.x,
                                   &_screen.y);

    _map_area.w = _screen.x;
    _map_area.h = _screen.y;

    _menu_area.x = _screen.x - _menu_area.w;
    _menu_area.y = _screen.y - _menu_area.h;

    render_map(rnd);
    if(_show_menu)
        render_menu(rnd);
}

void LevelEditor::render_map(SDL_Renderer* rnd)
{
    SDL_SetRenderDrawColor(rnd, 0, 32, 0, 255);
    SDL_RenderFillRect(rnd, &_map_area);
    SDL_SetRenderDrawColor(rnd, 0, 0, 0, 255);
    
    int tile_size = 32 * _map_zoom;
    SDL_Rect to {
        _map_offset.x,
        _map_offset.y,
        tile_size,
        tile_size
    };

    SDL_Point size = _map.size();
    string assets_dir = _engine->get_story().assets_dir;


    for(int x = 0; x < size.x; x++)
    {
        to.y = _map_offset.y;
        for(int y = 0; y < size.y; y++)
        {
            auto const& tile = _map.at(x, y);
            
            SDL_Rect from {
                tile.x * 32,
                tile.y * 32,
                32, 32
            };
            
            if(!tile.empty)
            {
                auto texture = _engine->get_texture
                    (assets_dir + tile.tileset->texture);

                SDL_RenderCopy(rnd, texture, &from, &to);
            }
            else
            {
                SDL_RenderFillRect(rnd, &to);
            }
            to.y += tile_size;
        }
        to.x += tile_size;
    }
}

void LevelEditor::render_menu(SDL_Renderer* rnd)
{
    SDL_SetRenderDrawColor(rnd, 32, 0, 32, 255);
    SDL_RenderFillRect(rnd, &_menu_area);
    SDL_RenderSetClipRect(rnd, &_menu_area);

    string uri = _engine->get_story().assets_dir
                + _tileset->texture;
    auto texture = _engine->get_texture(uri);

    SDL_Rect to = _menu_area;
    if(to.h > texture.h() * 1.5)
        to.h = texture.h() * 1.5;
    if(to.w > texture.w() * 1.5)
        to.w = texture.w() * 1.5;
    
    to.x += _tileset_offset.x;
    to.y += _tileset_offset.y;

    SDL_Rect from = {0, 0, to.w, to.h};
    from.w /= 1.5;
    from.h /= 1.5;

    SDL_RenderCopy(rnd, texture, &from, &to);
    
    SDL_Rect selection = _selection * 32 * 1.5;
    selection.x += _menu_area.x + _tileset_offset.x;
    selection.y += _menu_area.y + _tileset_offset.y;

    SDL_SetRenderDrawColor(rnd, 255, 255, 255, 255);
    SDL_RenderDrawRect(rnd, &selection);

    SDL_RenderSetClipRect(rnd, nullptr);
}
