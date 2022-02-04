#include "GameScene.hpp"
#include "StoryData.hpp"
#include "Engine.hpp"
#include <SDL2/SDL_render.h>
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <chrono>

using std::chrono::milliseconds;
using std::chrono::steady_clock;


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
