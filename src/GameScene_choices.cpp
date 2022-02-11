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

        other.bottom_margin += height - 5;
    }

    _speeches.push_back(speech);
    _wait_input = true;
}

void GameScene::
make_choice(SDL_MouseButtonEvent const& e)
{
    auto itr = _speeches.begin();
    while(itr != _speeches.end() && !itr->options)
        itr++;

    if(itr == _speeches.end())
    {
        print("No choice bubble found, story resumes.");
        _wait_input = false;
        return;
    }

    SDL_Point mouse_pos {e.x, e.y};
    SDL_Rect area {0, 0, 0, -5};
    area.x = _camera_offset.x + itr->pos.x * _zoom * 32;
    area.y = _camera_offset.y + itr->pos.y * _zoom * 32;
    

    for(auto pair : *itr->options)
    {
        if(area.w < pair.first.w())
            area.w = pair.first.w();
        area.h += pair.first.h() + 5;
    }

    area.w += 16;
    auto speech_height = area.h + 5;

    area.x -= area.w / 2;
    area.y -= area.h;
    
    for(auto pair : *itr->options)
    {
        area.h = pair.first.h() + 5;
        if(SDL_PointInRect(&mouse_pos, &area))
        {
            print("You hit speach. ", pair.first.w());
            return;
            run_story_arc(pair.second);
            _speeches.erase(itr);
            for(auto& speech : _speeches)
                speech.bottom_margin
                        -= speech_height;
            _wait_input = false;
            return;
        }
        area.y += area.h;
    }
    print("You no hit speach.");
}

