#include "GameScene.hpp"
#include "Map.hpp"
#include <SDL2/SDL_render.h>
#define ENABLE_PRINTING PRINT_GAME_LOG
//#include "print.hpp"


void GameScene::render(SDL_Renderer* rnd)
{
    SDL_GetRendererOutputSize(rnd, &_screen_size.x,
                                   &_screen_size.y);
    if(_map)
    {
        fit_map_on_screen();
        render_map(rnd);
    }
}

void GameScene::fit_map_on_screen()
{
    auto const& screen = _screen_size;
    auto& offset = _camera_offset;
    auto map = _map->size;
    map.x *= 32;
    map.y *= 32;

    /* Ensure no black space's on screen */
    if(offset.x > 0) offset.x = 0;
    if(offset.y > 0) offset.y = 0;
    if(offset.x < screen.x - map.x)
        offset.x = screen.x - map.x;
    if(offset.y < screen.y - map.y)
        offset.y = screen.y - map.y;


    /* Center on larger screen */
    if(map.x < screen.x)
        offset.x = (screen.x - map.x) / 2;
    if(map.y < screen.y)
        offset.y = (screen.y - map.y) / 2;
}

void GameScene::render_map(SDL_Renderer* rnd)
{
    SDL_Rect from {0, 0, 32, 32};
    SDL_Rect to {0, 0, 32, 32};

    auto const& map_size = _map->size;
    int map_i = 0;
    for(auto tile : _map->data)
    {
        auto& cache = _tilesets[tile.tileset];
        auto& size = cache.second;
        
        SDL_Point pos;
        if(tile.mode == Map::TileData::mode_pos)
        {
            pos.x = tile.tile.pos.x;
            pos.y = tile.tile.pos.y;
        }
        else
        {
            auto id = tile.tile.id;
            pos.x = id % size.y;
            pos.y = id / size.y;
        }

        from.x = pos.x * from.w;
        from.y = pos.y * from.h;
        
        auto map_x = map_i % map_size.x;
        auto map_y = map_i / map_size.x;

        to.x = _camera_offset.x + map_x * to.w;
        to.y = _camera_offset.y + map_y * to.h;


        SDL_Texture* img = cache.first.get();
        SDL_RenderCopy(rnd, img, &from, &to);
        map_i++;
        if(map_i >= map_size.x * map_size.y)
            return;
    }
}
