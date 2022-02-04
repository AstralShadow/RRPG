#include "GameScene.hpp"
#include "StoryData.hpp"
#include "Engine.hpp"
#include "Map.hpp"
#include <SDL2/SDL_render.h>
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::steady_clock;


void GameScene::render(SDL_Renderer* rnd)
{
    SDL_GetRendererOutputSize(rnd, &_screen_size.x,
                                   &_screen_size.y);
    if(_map)
    {
        fit_map_on_screen();
        render_map(rnd);
        render_entities(rnd);
        render_speeches(rnd);
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

        from.x = frame % entity.sprite->size.x;
        from.y = frame / entity.sprite->size.x;
        from.x *= from.w;
        from.y *= from.h;


        SDL_RenderCopy(rnd, entity.texture, &from, &to);
    }
}

void GameScene::render_speeches(SDL_Renderer* rnd)
{
    auto now = steady_clock::now();

    SDL_Rect to;
    for(auto& speech : _speeches)
    {
        to.x = speech.pos.x * _zoom * 32;
        to.y = speech.pos.y * _zoom * 32;
        to.x += _camera_offset.x;
        to.y += _camera_offset.y;
        to.w = speech.text.w();
        to.h = speech.text.h();
        to.x -= to.w / 2;
        to.y -= to.h / 2;

        auto entity = _entities.find(speech.entity);
        Point* pos = nullptr;
        if(entity != _entities.end())
            pos = &entity->second.pos;

        auto age = duration_cast<milliseconds>
                (now - speech.created);

        uint8_t alpha = 255;
        if(age.count() > 3000)
        {
            int opacity = 4000 - age.count();
            if(opacity < 0) opacity = 0;
            alpha = opacity * 32 / 125;
        }
        if(age.count() < 500)
        {
            auto width = to.w * age.count() / 500;
            to.x += (to.w - width) / 2;
            to.w = width;
        }
        
        if(age.count() < 0)
            continue;


        SDL_Rect from {0, 0, to.w, to.h};
        render_speech_bubble(rnd, to, pos, alpha);
        SDL_SetTextureAlphaMod(speech.text, alpha);
        SDL_RenderCopy(rnd, speech.text, &from, &to);
        SDL_SetTextureAlphaMod(speech.text, 255);
    }
}

/*
 * Assuming our speech bubble is 4x3 tiles. Content:
 * [edge]   [t line] [top line + poitner] [edge]
 * [l line] [bg]     [bg + pointer]       [r wall]
 * [edge]   [b line] [b line + pointer]   [edge]
 */
void GameScene::
render_speech_bubble(SDL_Renderer* rnd,
                     SDL_Rect area,
                     Point* entity_pos,
                     uint8_t alpha)
{
    static Texture texture = _engine->get_texture
        (_data->assets_dir + "img/speech_bubble.png");
    SDL_SetTextureAlphaMod(texture, alpha);
    
    render_speech_bubble_edges
        (rnd, texture, area);
    render_speech_bubble_vertical_borders
        (rnd, texture, area);

    if(entity_pos)
    {
        int ptr_pos = (entity_pos->x * 32 + 16) * _zoom;
        ptr_pos += _camera_offset.x;
        render_speech_bubble_horizontal_borders
            (rnd, texture, area, ptr_pos);
        render_speech_bubble_background
            (rnd, texture, area, ptr_pos);
    }
    else
    {
        render_speech_bubble_horizontal_borders
            (rnd, texture, area);
        render_speech_bubble_background
            (rnd, texture, area);
    }
}

void GameScene::
render_speech_bubble_edges(SDL_Renderer* rnd,
                           Texture& texture,
                           SDL_Rect area)
{
    SDL_Rect from {0, 0, 32 ,32};
    SDL_Rect to {0, 0, 32, 32};

    to.x = area.x - 32;
    to.y = area.y - 32;
    SDL_RenderCopy(rnd, texture, &from, &to);
    from.x = 96;
    to.x = area.x + area.w;
    SDL_RenderCopy(rnd, texture, &from, &to);
    from.y = 64;
    to.y = area.y + area.h;
    SDL_RenderCopy(rnd, texture, &from, &to);
    from.x = 0;
    to.x = area.x - 32;
    SDL_RenderCopy(rnd, texture, &from, &to);
}

void GameScene::
render_speech_bubble_vertical_borders(SDL_Renderer* rnd,
                                      Texture& texture,
                                      SDL_Rect area)
{
    SDL_Rect to {area.x, area.y, 32, 32};
    SDL_Rect from {0, 32, 32, 32};

    int left_h = area.h;
    while(left_h > 0)
    {
        left_h -= 32;
        int h = left_h > 0 ? 32 : 32 + left_h;
        from.h = h; to.h = h;
        
        to.x = area.x - 32;
        from.x = 0;
        SDL_RenderCopy(rnd, texture, &from, &to);
        to.x = area.x + area.w;
        from.x = 96;
        SDL_RenderCopy(rnd, texture, &from, &to);
        
        to.y += 32;
    }
}

void GameScene::render_speech_bubble_horizontal_borders
   (SDL_Renderer* rnd,
    Texture& texture,
    SDL_Rect area)
{
    SDL_Rect to {area.x, area.y, 32, 32};
    SDL_Rect from {32, 0, 32, 32};

    int left_w = area.w;
    while(left_w > 0)
    {
        left_w -= 32;
        int w = left_w > 0 ? 32 : 32 + left_w;
        from.w = w;
        to.w = w;
        
        from.y = 0;
        to.y = area.y - 32;
        SDL_RenderCopy(rnd, texture, &from, &to);
        from.y = 64;
        to.y = area.y + area.h;
        SDL_RenderCopy(rnd, texture, &from, &to);
        
        to.x += 32;
    }
}

void GameScene::render_speech_bubble_horizontal_borders
   (SDL_Renderer* rnd,
    Texture& texture,
    SDL_Rect area,
    int ptr_pos)
{
    SDL_Rect left_area = area;
    SDL_Rect right_area = area;
    left_area.w = ptr_pos - area.x - 16;
    right_area.x += left_area.w + 32;
    right_area.w = area.w - (ptr_pos - area.x + 16);
    render_speech_bubble_horizontal_borders
        (rnd, texture, left_area);
    render_speech_bubble_horizontal_borders
        (rnd, texture, right_area);


    SDL_Rect to {
        left_area.x + left_area.w,
        area.y,
        32, 32
    };

    SDL_Rect from {64, 0, 32, 32};

    if(area.w < 32)
    {
        to.x += (32 - area.w) / 2;
        to.w -= (32 - area.w);
        from.x += (32 - area.w) / 2;
        from.w = to.w;
    }
    
    from.y = 0;
    to.y = area.y - 32;
    SDL_RenderCopy(rnd, texture, &from, &to);

    to.y = area.y + area.h;
    from.y = 64;
    SDL_RenderCopy(rnd, texture, &from, &to);
}

void GameScene::
render_speech_bubble_background(SDL_Renderer* rnd,
                                Texture& texture,
                                SDL_Rect area)
{
    SDL_Rect to {area.x, area.y, 32, 32};
    SDL_Rect from {32, 32, 32, 32};

    int left_w = area.w;
    while(left_w > 0)
    {
        left_w -= 32;
        int w = left_w > 0 ? 32: 32 + left_w;
        from.w = w; to.w = w;

        int left_h = area.h;
        to.y = area.y;
        while(left_h > 0)
        {
            left_h -= 32;
            int h = left_h > 0 ? 32 : 32 + left_h;
            from.h = h; to.h = h;

            SDL_RenderCopy(rnd, texture, &from, &to);

            to.y += 32;
        }

        to.x += 32;
    }
}

void GameScene::
render_speech_bubble_background(SDL_Renderer* rnd,
                                Texture& texture,
                                SDL_Rect area,
                                int ptr_pos)
{
    SDL_Rect left_area = area;
    SDL_Rect right_area = area;
    left_area.w = ptr_pos - area.x - 16;
    right_area.x += left_area.w + 32;
    right_area.w = area.w - (ptr_pos - area.x + 16);
    render_speech_bubble_background
        (rnd, texture, left_area);
    render_speech_bubble_background
        (rnd, texture, right_area);


    SDL_Rect to {
        left_area.x + left_area.w,
        area.y,
        32, 32
    };

    SDL_Rect from {64, 32, 32, 32};

    if(area.w < 32)
    {
        to.w = area.w;
        from.w = to.w;
        from.x += (32 - to.w) / 2;
    }

    int left_h = area.h;
    while(left_h > 0)
    {
        left_h -= 32;
        int h = left_h > 0 ? 32 : 32 + left_h;
        from.h = h; to.h = h;
        
        SDL_RenderCopy(rnd, texture, &from, &to);
        
        to.y += 32;
    }
}
