#include "MainMenu.hpp"
#include "Engine.hpp"
#define ENABLE_PRINTING 1
#include "print.hpp"
#include <cmath>
#include <SDL2/SDL_render.h>


MainMenu::MainMenu(Engine* e) :
    Scene(e),
    _ani_progress(0)
{ }


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

    _art_offset_x = _ani_progress.count() / 42;

}

void MainMenu::render(SDL_Renderer* rnd)
{
    SDL_GetRendererOutputSize(rnd, &_screen_size.x,
                                   &_screen_size.y);

    render_art(rnd);

    SDL_RenderCopy(rnd, _play, nullptr, &_play_area);
}

void MainMenu::render_art(SDL_Renderer* rnd)
{
    float w_h = _screen_size.x / (float) _screen_size.y;
    float scale = _screen_size.y / (float) _art.h();
    print(scale);

    int offset = _art_offset_x % _art.w();
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
        cut_w * scale, _screen_size.y
    };

    SDL_RenderCopy(rnd, _art, &from, &to);

    to.x = to.w;
    to.w = _screen_size.x - to.x;

    from.x = 0;
    from.w = to.w / scale;

    SDL_RenderCopy(rnd, _art, &from, &to);
}

void MainMenu::process(SDL_Event const&)
{

}
