#include "GameScene.hpp"
#include "Actions.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <memory>
#include <stdexcept>
#include <chrono>


using std::string;
using std::shared_ptr;
using std::runtime_error;
using std::chrono::steady_clock;
using std::chrono::seconds;


static const SDL_Color color {0, 0, 0, 255};


void GameScene::create_speech(shared_ptr<Speech> cmd)
{
    SpeechBubble speech;
    speech.entity = cmd->entity;
    speech.text = _engine->get_text(cmd->text, color);
    speech.created = steady_clock::now();

    speech.pos = _screen_size;
    speech.pos.x /= 2;
    speech.pos.y /= 2;

    auto entity = _entities.find(speech.entity);
    if(entity != _entities.end())
    {
        std::uniform_real_distribution<float>
            distribution1(-0.3, 0.3);
        std::uniform_real_distribution<float>
            distribution2(-0.05, 0.05);
        speech.pos.x = entity->second.pos.x + 0.5;
        speech.pos.y = entity->second.pos.y - 0.25;
        speech.pos.x += distribution1(mt_generator);
        speech.pos.y += distribution2(mt_generator);
    }


    print("Addin' speech bubblo");
    _speeches.push_back(speech);
}

void GameScene::remove_old_speeches()
{
    auto now = steady_clock::now();
    auto itr = _speeches.begin();
    while(itr != _speeches.end())
    {
        auto age = now - itr->created;
        if(age > seconds(5))
        {
            itr = _speeches.erase(itr);
            continue;
        }
        itr++;
    }
}

void GameScene::position_speeches(duration_t progress)
{
    
}

