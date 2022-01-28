#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


Game::Game() :
    _running(false),
    _window(nullptr),
    _renderer(nullptr)
{}

Game::~Game()
{
    IMG_Quit();
    SDL_Quit();
}

void Game::run()
{

}
