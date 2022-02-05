#include "GameScene.hpp"
#include "Actions.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <memory>
#include <stdexcept>
#include <chrono>
#include <SDL2/SDL_events.h>


using std::string;
using std::shared_ptr;
using std::runtime_error;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


static const SDL_Color color {0, 0, 0, 255};


void GameScene::create_choice(shared_ptr<Choice> cmd)
{
    SpeechBubble speech;
    speech.entity = cmd->entity;
    speech.age = duration_t(0);
    speech.options = std::make_shared
                     <SpeechBubble::ChoiceOptions>();

    float height = 16.0f;
    for(auto& pair : cmd->options)
    {
        auto text = _engine->get_text(pair.first, color);
        speech.options->emplace_back(text, &pair.second);
        height += text.h() + 5;
    }

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

        other.bottom_margin += height;
    }

    _speeches.push_back(speech);
    _wait_input = true;
}

void GameScene::process(SDL_MouseButtonEvent const& e)
{
    print("Ya attemtped ta click");
}

