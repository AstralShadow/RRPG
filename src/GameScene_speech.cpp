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
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


static const SDL_Color color {0, 0, 0, 255};


void GameScene::create_speech(shared_ptr<Speech> cmd)
{
    SpeechBubble speech;
    speech.entity = cmd->entity;
    speech.text = _engine->get_text(cmd->text, color);
    speech.age = duration_t(0);

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

    for(auto& other: _speeches)
    {
        if(other.entity != speech.entity)
            continue;

        other.same_entity_speeches_after_this++;
    }

    _speeches.push_back(speech);
}

void GameScene::remove_old_speeches(duration_t progress)
{
    auto itr = _speeches.begin();
    while(itr != _speeches.end())
    {
        itr->age += progress;
        if(itr->age > seconds(5) && !itr->options)
        {
            itr = _speeches.erase(itr);
            continue;
        }
        itr++;
    }
}

void GameScene::position_speeches()
{
    position_speeches_horizontally();
    position_speeches_vertically();
}

void GameScene::position_speeches_horizontally()
{
    for(auto& speech : _speeches)
    {
        auto entity_itr = _entities.find(speech.entity);
        if(entity_itr == _entities.end()) continue;
        auto& entity = entity_itr->second;
        float entity_x = entity.pos.x;
        
        int speech_w = speech.text.w();
        auto age = duration_cast
                    <milliseconds>(speech.age);
        if(age < milliseconds(500))
            speech_w *= age.count() / 500;

        float speech_x1 = speech.pos.x
                - speech_w / (_zoom * 64);

        if(speech_x1 > entity_x)
            speech.pos.x -= speech_x1 - entity_x;

        float speech_x2 = speech.pos.x
                + speech_w / (_zoom * 64);

        if(speech_x2 < entity_x + 1)
            speech.pos.x += entity_x + 1 - speech_x2;

        if(speech_x1 > speech_x2 - 1)
            speech.pos.x = entity_x + 0.5;
    }
}

void GameScene::position_speeches_vertically()
{
    for(auto& speech : _speeches)
    {
        auto entity_itr = _entities.find(speech.entity);
        if(entity_itr == _entities.end()) continue;
        auto& entity = entity_itr->second;
        int id = speech.same_entity_speeches_after_this;
        speech.pos.y = entity.pos.y - 0.3;
        speech.pos.y -= id * 1.1 / _zoom;
        
    }
}
