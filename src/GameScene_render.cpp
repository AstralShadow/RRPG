#include "GameScene.hpp"
#include "StoryData.hpp"
#include "Engine.hpp"
#include "Map.hpp"
#include <SDL2/SDL_render.h>
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"

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
    SDL_FRect to { 0, 0, 32 * _zoom, 32 * _zoom };
    SDL_Rect from {0, 0, 32, 32};

    auto const& map_size = _map->size;
    int map_i = 0;
    for(auto tile : _map->data)
    {
        auto& cache = _tilesets[tile.tileset];
        auto& size = cache.second;
        
        SDL_Point pos;
        switch(tile.mode)
        {
            case Map::TileData::mode_pos:
            {
                pos.x = tile.tile.pos.x;
                pos.y = tile.tile.pos.y;
                break;
            }

            case Map::TileData::mode_id:
            {
                auto id = tile.tile.id;
                pos.x = id % size.y;
                pos.y = id / size.y;
                break;
            }

            case Map::TileData::mode_empty:
            {
                map_i++;
                if(map_i >= map_size.x * map_size.y)
                    return;
                continue;
            }
        }

        from.x = pos.x * from.w;
        from.y = pos.y * from.h;
        
        auto map_x = map_i % map_size.x;
        auto map_y = map_i / map_size.x;

        to.x = _camera_offset.x + map_x * to.w;
        to.y = _camera_offset.y + map_y * to.h;

        SDL_Rect to2 = {(int)to.x, (int)to.y,
                        (int)to.w + 1, (int)to.h + 1};

        SDL_RenderCopy(rnd, cache.first, &from, &to2);

        if(_show_grid_locations)
        {
            const SDL_Color color {255, 255, 255, 255};
            auto text = _engine->get_text
                (std::to_string(map_x) + "x"
                 + std::to_string(map_y), color);
            SDL_Rect id_area { to2.x, to2.y,
                               text.w(), text.h() };
            SDL_RenderCopy(rnd, text, nullptr, &id_area);
        }

        map_i++;
        if(map_i >= map_size.x * map_size.y)
            return;
    }
}

void GameScene::render_entities(SDL_Renderer* rnd)
{
    for(auto pair : _entities)
    {
        auto const& entity = pair.second;

        int tile_size_x = entity.texture.w() /
                    entity.sprite->size.x;
        int tile_size_y = entity.texture.h() /
                    entity.sprite->size.y;
        SDL_Rect to { 0, 0,
                     (int) (tile_size_x * _zoom),
                     (int) (tile_size_y * _zoom) };
        SDL_Rect from {0, 0, tile_size_x, tile_size_y};

        to.x = _camera_offset.x +
            entity.pos.x * 32 * _zoom;
        to.y = _camera_offset.y +
            entity.pos.y * 32 * _zoom;
        
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
    SDL_Rect to;
    for(auto& speech : _speeches)
    {
        to.x = speech.pos.x * _zoom * 32;
        to.y = speech.pos.y * _zoom * 32;
        to.x += _camera_offset.x;
        to.y += _camera_offset.y;
        if(!speech.options)
        {
            to.w = speech.text.w();
            to.h = speech.text.h();
        }
        else
        {
            to.w = 0;
            to.h = -5;
            for(auto pair : *speech.options)
            {
                auto texture = pair.first;
                if(texture.w() > to.w)
                    to.w = texture.w();
                to.h += 5 + texture.h();
            }
        }
        to.x -= to.w / 2;
        to.y -= to.h;

        auto entity = _entities.find(speech.entity);
        Point* pos = nullptr;
        if(entity != _entities.end())
            pos = &entity->second.pos;
        if(speech.bottom_margin > 0)
            pos = nullptr;

        auto age = duration_cast
                    <milliseconds>(speech.age);

        uint8_t alpha = 255;
        if(age.count() > 3000 && !speech.options)
        {
            int opacity = 4000 - age.count();
            if(opacity < 0) opacity = 0;
            alpha = opacity * 32 / 125;
        }
        if(age.count() < 1000)
        {
            auto width = to.w * age.count() / 1000;
            to.x += (to.w - width) / 2;
            to.w = width;
        }
        
        if(age.count() < 0)
            continue;

        
        string texture_uri = _data->assets_dir;
        if(speech.options)
            texture_uri += "img/choice_bubble.png";
        else
            texture_uri += "img/speech_bubble.png";

        auto texture = _engine->get_texture(texture_uri);
        SDL_SetTextureAlphaMod(texture, alpha);
        render_speech_bubble(rnd, texture, to, pos);
        SDL_SetTextureAlphaMod(texture, 255);

        if(!speech.options)
        {
            SDL_Rect from {0, 0, to.w, to.h};

            SDL_SetTextureAlphaMod(speech.text, alpha);
            SDL_RenderCopy(rnd, speech.text, &from, &to);
            SDL_SetTextureAlphaMod(speech.text, 255);
        }
        else
        {
            auto bubble_width = to.w;
            for(auto pair : *speech.options)
            {
                auto text = pair.first;
                SDL_SetTextureAlphaMod(text, alpha);
                
                to.h = text.h();
                to.w = std::min(text.w(), bubble_width);

                SDL_Rect from {0, 0, to.w, to.h};
                SDL_RenderCopy(rnd, text, &from, &to);

                to.y += text.h() + 5;

                SDL_SetTextureAlphaMod(text, 255);
            }
        }
    }
}

