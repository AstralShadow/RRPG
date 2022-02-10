#include "Engine.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define ENABLE_PRINTING 1
#include "print.hpp"

namespace EngineModeLog
{
    #ifdef ENABLE_PRINTING
        #undef ENABLE_PRINTING
    #endif
    #define ENABLE_PRINTING PRINT_ENGINE_MODE
    #include "print.hpp"
}


Engine::Engine() :
    _running(false),
    _window(nullptr),
    _renderer(nullptr)
{
    
}

Engine::~Engine()
{
    clear_textures();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void Engine::set_scene(EngineMode mode,
                      shared_ptr<Scene> scene)
{
    _scenes[mode] = scene;
    if(mode == _mode)
        scene->on_enter();
}


void Engine::set_mode(EngineMode mode)
{
    using EngineModeLog::print;
    switch(mode)
    {
        case EngineMode::loading:
            print("Engine mode: loading");
            break;
        case EngineMode::menu:
            print("Engine mode: menu");
            break;
        case EngineMode::playing:
            print("Engine mode: playing");
            break;
        case EngineMode::credits:
            print("Engine mode: creadits");
            break;
        case EngineMode::map_editor:
            print("Engine mode: Map editor");
            break;
        default:
            print("Engine mode: unnamed");
            break;
    }

    _mode = mode;
    if(_scenes.find(_mode) != _scenes.end())
        _scenes[_mode]->on_enter();

}


void Engine::run()
{
    using std::chrono::steady_clock;
    using std::chrono::duration_cast;

    _running = true;
    auto last_tick = steady_clock::now();

    while(_running)
    {
        auto tick_start = steady_clock::now();
        
        duration_t progress = duration_cast
                <duration_t>(tick_start - last_tick);
        progress = duration_cast
                <duration_t>(progress * GAME_SPEED);

        last_tick = tick_start;

        poll_events();
        tick(progress);
        render();

        auto tick_end = steady_clock::now();
        auto tick_time = tick_end - tick_start;
        auto sleep_time = TICK_INTERVAL - tick_time;
        
        #if PRINT_TICK_SLEEP_TIME
            std::cout << "Sleeping for "
                << duration_cast<duration_t>(sleep_time)
                    .count() << std::endl;
        #endif

        std::this_thread::sleep_for(sleep_time);
    }
}

void Engine::poll_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                stop();
                break;
            default:
                if(_scenes.find(_mode) != _scenes.end())
                    _scenes[_mode]->process(event);
                break;
        }
    }
}

void Engine::tick(duration_t progress)
{
    if(_scenes.find(_mode) != _scenes.end())
        _scenes[_mode]->tick(progress);
}

void Engine::render()
{
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    if(_scenes.find(_mode) != _scenes.end())
        _scenes[_mode]->render(_renderer);

    SDL_RenderPresent(_renderer);
}

void Engine::stop()
{
    #if PRINT_EXIT_MESSAGE 
        std::cout << "Stopping the game." << std::endl;
    #endif
    _running = false;
}
