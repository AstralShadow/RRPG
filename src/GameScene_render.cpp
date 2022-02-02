#include "GameScene.hpp"
#include "Map.hpp"
#include <SDL2/SDL_render.h>
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"

using std::chrono::duration_cast;
using std::chrono::seconds;


void GameScene::render(SDL_Renderer* rnd)
{
    SDL_GetRendererOutputSize(rnd, &_screen_size.x,
                                   &_screen_size.y);
    if(_map)
    {
        fit_map_on_screen();
        render_map(rnd);
        render_entities(rnd);
    }
}

void GameScene::fit_map_on_screen()
{
    auto const& screen = _screen_size;
    auto& offset = _camera_offset;
    auto map = _map->size;
    map.x = map.x * 32 * _zoom;
    map.y = map.y * 32 * _zoom;

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
    SDL_Rect to { 0, 0,
                 (int) (32 * _zoom),
                 (int) (32 * _zoom) };
    SDL_Rect from {0, 0, 32, 32};

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


        SDL_RenderCopy(rnd, cache.first, &from, &to);
        map_i++;
        if(map_i >= map_size.x * map_size.y)
            return;
    }
}

void GameScene::render_entities(SDL_Renderer* rnd)
{
    SDL_Rect to { 0, 0,
                 (int) (32 * _zoom),
                 (int) (32 * _zoom) };
    SDL_Rect from {0, 0, 32, 32};

    for(auto pair : _entities)
    {
        auto const& entity = pair.second;
        to.x = _camera_offset.x + entity.pos.x * to.w;
        to.y = _camera_offset.y + entity.pos.y * to.h;
        
        auto ani_data = entity.sprite
                            ->animation[entity.state];
        auto frame = (duration_cast<seconds>
            (entity.animation_progress * ani_data.speed)
            .count()) % ani_data.frames;

        print("ani frame: ",frame, "(",entity.state,")");
        switch(entity.direction)
        {
            case Direction::LEFT:
                frame += ani_data.left;
                break;
            case Direction::DOWN:
                frame += ani_data.down;
                break;
            case Direction::UP:
                frame += ani_data.up;
                break;
            case Direction::RIGHT:
                frame += ani_data.right;
                break;
        }
        print("ani frame total: ", frame);

        from.x = frame % entity.sprite->size.x;
        from.y = frame / entity.sprite->size.x;
        print("ani frame pos: ", from.x, ":", from.y);
        from.x *= from.w;
        from.y *= from.h;


        SDL_RenderCopy(rnd, entity.texture, &from, &to);
    }
}

