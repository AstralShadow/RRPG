#include "GameScene.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"

using std::chrono::milliseconds;


GameScene::GameScene(Engine* engine) :
    Scene(engine),
    _story(&engine->get_story())
{
    
}

GameScene::~GameScene()
{

}

void GameScene::on_enter()
{

}

void GameScene::tick(milliseconds progress)
{

}

void GameScene::render(SDL_Renderer* renderer)
{

}

void GameScene::process(SDL_Event const&)
{
    print("i no parse event!!");
}
