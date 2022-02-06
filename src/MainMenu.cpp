#include "MainMenu.hpp"
#include "Engine.hpp"
#define ENABLE_PRINTING 1
#include "print.hpp"
#include <cmath>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>


MainMenu::MainMenu(Engine* e) :
    Scene(e),
    _ani_progress(0)
{ }

MainMenu::~MainMenu()
{
    if(_art_cache)
        SDL_DestroyTexture(_art_cache);
}


void MainMenu::on_enter()
{
    string root = _engine->get_story().assets_dir;
    _art = _engine->get_texture
            (root + "img/background.png");
    _play = _engine->get_texture
            (root + "img/play.png");

    _play_area.w = _play.w();
    _play_area.h = _play.h();
}

void MainMenu::tick(duration_t progress)
{
    _ani_progress += progress;
    
    float play_pos = sin(_ani_progress.count() / 1000.0);

    _play_area.x = (_screen_size.x - _play_area.w) / 2;
    _play_area.y = (_screen_size.y - _play_area.h) - 16;
    _play_area.y += play_pos * _play_motion_height / 2;

    _art_offset_x = _ani_progress.count() / 50000.0;

}

void MainMenu::render(SDL_Renderer* rnd)
{
    int old_screen_height = _screen_size.y;
    SDL_GetRendererOutputSize(rnd, &_screen_size.x,
                                   &_screen_size.y);


    SDL_RendererInfo info;
    SDL_GetRendererInfo(rnd, &info);
    if(info.flags & SDL_RENDERER_TARGETTEXTURE)
    {
        if(old_screen_height != _screen_size.y)
            update_art_cache(rnd);
        render_cache_art(rnd);
    }
    else
        render_art(rnd);

    SDL_RenderCopy(rnd, _play, nullptr, &_play_area);
}

void MainMenu::render_art(SDL_Renderer* rnd)
{
    float w_h = _screen_size.x / (float) _screen_size.y;
    float scale = _screen_size.y / (float) _art.h();

    int offset = (int)(_art_offset_x * _art.w())
                    % _art.w();
    int art_h = _art.h();
    int art_w = art_h * w_h;
    int cut_w = _art.w() - offset;

    if(cut_w > art_w)
    {
        SDL_Rect from {
            offset, 0,
            art_w, art_h
        };
        SDL_RenderCopy(rnd, _art, &from, nullptr);
        return;
    }

    SDL_Rect from {
        offset, 0,
        cut_w, art_h
    };

    SDL_Rect to {
        0, 0,
        (int) (cut_w * scale),
        _screen_size.y
    };

    SDL_RenderCopy(rnd, _art, &from, &to);

    to.x = to.w;
    to.w = _screen_size.x - to.x;

    from.x = 0;
    from.w = to.w / scale;

    SDL_RenderCopy(rnd, _art, &from, &to);
}

void MainMenu::render_cache_art(SDL_Renderer* rnd)
{
    float w_h = _screen_size.x / (float) _screen_size.y;

    int offset = (int)(_art_offset_x * _cache_size.x)
                    % _cache_size.x; 
    int art_h = _cache_size.y;
    int art_w = art_h * w_h;
    int cut_w = _cache_size.x - offset;

    if(cut_w > art_w)
    {
        SDL_Rect from {
            offset, 0,
            art_w, art_h
        };
        SDL_RenderCopy(rnd, _art_cache, &from, nullptr);
        return;
    }

    SDL_Rect from {
        offset, 0,
        cut_w, art_h
    };

    SDL_Rect to {
        0, 0,
        cut_w, _screen_size.y
    };

    SDL_RenderCopy(rnd, _art_cache, &from, &to);

    to.x = to.w;
    to.w = _screen_size.x - to.x;

    from.x = 0;
    from.w = to.w;

    SDL_RenderCopy(rnd, _art_cache, &from, &to);
}

void MainMenu::update_art_cache(SDL_Renderer* rnd)
{
    if(_art_cache)
        SDL_DestroyTexture(_art_cache);

    _cache_size.x = _art.w() * _screen_size.y / _art.h();
    _cache_size.y = _screen_size.y;
    
    _art_cache = SDL_CreateTexture (rnd,
         SDL_PIXELFORMAT_RGBA8888,
         SDL_TEXTUREACCESS_TARGET,
         _cache_size.x,
         _cache_size.y);


    SDL_SetRenderTarget(rnd, _art_cache);

    SDL_RenderCopy(rnd, _art, nullptr, nullptr);

    SDL_SetRenderTarget(rnd, nullptr);
}


void MainMenu::process(SDL_Event const& e)
{
    switch(e.type)
    {
        #if BUILD_LEVEL_EDITOR == 2
        case SDL_KEYDOWN:
        {
            if(e.key.keysym.scancode == SDL_SCANCODE_M)
                _engine->set_mode
                    (EngineMode::map_editor);
            break;
        }
        #endif

        case SDL_MOUSEBUTTONUP:
        {
            SDL_Point pos {e.button.x, e.button.y};
            if(SDL_PointInRect(&pos, &_play_area))
                _engine->set_mode
                    (EngineMode::playing);

            break;
        }

        default: break;
    }
}
