#include "GameScene.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"

using std::chrono::milliseconds;
using std::string;


GameScene::GameScene(Engine* engine, string main_story) :
    Scene(engine),
    _data(&engine->get_story())
{ }

GameScene::~GameScene()
{ }


void GameScene::on_enter()
{ }

void GameScene::tick(milliseconds progress)
{

}

void GameScene::render(SDL_Renderer* renderer)
{

}

void GameScene::process(SDL_Event const&)
{

}
