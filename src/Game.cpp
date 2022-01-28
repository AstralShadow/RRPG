#include "Game.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


Game::Game() :
    _running(false),
    _window(nullptr),
    _renderer(nullptr)
{}

Game::~Game()
{
    clear_textures();
    IMG_Quit();
    SDL_Quit();
}

void Game::run()
{
    using std::chrono::steady_clock;
    using std::chrono::duration_cast;

    _running = true;
    auto last_tick = steady_clock::now();

    while(_running)
    {
        auto tick_start = steady_clock::now();
        auto progress = tick_start - last_tick;
        last_tick = tick_start;

        poll_events();
        tick(duration_cast<duration_t>(progress));
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

void Game::poll_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                stop();
                break;
        }
    }
}

void Game::tick(duration_t progress)
{

}

void Game::render()
{

}

void Game::stop()
{
    #if PRINT_EXIT_MESSAGE 
        std::cout << "Stopping the game." << std::endl;
    #endif
    _running = false;
}
