#include "LevelEditor.hpp"
#include "Engine.hpp"
#include <SDL2/SDL_render.h>


void LevelEditor::render(SDL_Renderer* rnd)
{
    SDL_GetRendererOutputSize(rnd, &_screen.x,
                                   &_screen.y);

    _map.w = _screen.x;
    _map.h = _screen.y;

    _menu.x = _screen.x - _menu.w;
    _menu.y = _screen.y - _menu.h;

    render_map(rnd);
    if(_show_menu)
        render_menu(rnd);
}

void LevelEditor::render_map(SDL_Renderer* rnd)
{
    SDL_SetRenderDrawColor(rnd, 0, 100, 0, 255);
    SDL_RenderFillRect(rnd, &_map);

}

void LevelEditor::render_menu(SDL_Renderer* rnd)
{
    SDL_SetRenderDrawColor(rnd, 32, 0, 32, 255);
    SDL_RenderFillRect(rnd, &_menu);
    SDL_RenderSetClipRect(rnd, &_menu);

    string uri = _engine->get_story().assets_dir
                + _tileset->texture;
    auto texture = _engine->get_texture(uri);

    SDL_Rect to = _menu;
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
    selection.x += _menu.x + _tileset_offset.x;
    selection.y += _menu.y + _tileset_offset.y;

    SDL_SetRenderDrawColor(rnd, 255, 255, 255, 255);
    SDL_RenderDrawRect(rnd, &selection);

    SDL_RenderSetClipRect(rnd, nullptr);
}
