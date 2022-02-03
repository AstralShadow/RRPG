#include "GameScene.hpp"
#include "Engine.hpp"
#include "StoryData.hpp"
#define ENABLE_PRINTING PRINT_GAME_LOG
#include "print.hpp"
#include <SDL2/SDL_events.h>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::string;


GameScene::GameScene(Engine* engine, string main_story) :
    Scene(engine),
    _data(&engine->get_story()),
    _action_end(steady_clock::now()),
    _camera_offset{0, 0}
{
    set_story(main_story);
}

GameScene::~GameScene()
{ }

void GameScene::on_enter()
{
    _wait_input = false;
}

void GameScene::tick(duration_t progress)
{
    update_animations(progress);
    update_motions(progress);

    auto start = steady_clock::now();
    while(!_wait_input &&
          _action_end < steady_clock::now())
    {
        process_action();
        #if DISABLE_BATCH_PROGRESS_EXECUTION
            break;
        #endif

        auto now = steady_clock::now();
        auto duration = duration_cast
                    <milliseconds>(now - start);

        if(duration.count() > 5)
        break;
    }
}

void GameScene::sleep(milliseconds time)
{
    _action_end = steady_clock::now() + time;
}

void GameScene::update_animations(duration_t progress)
{
    for(auto& pair : _entities)
    {
        auto& entity = pair.second;
        entity.animation_progress += progress;
    }
}

void GameScene::update_motions(duration_t progress)
{
    for(auto& motion : _motions)
        motion.progress += progress;

    auto prev = _motions.before_begin();
    auto itr = _motions.begin();

    while(itr != _motions.end())
    {
        auto& motion = *itr;
        if(motion.progress >= motion.length)
        {
            finish_motion(motion);
            itr = _motions.erase_after(prev);
            continue;
        }

        float m_progress = motion.progress.count();
        m_progress /= motion.length.count();

        if(m_progress > 1.0) m_progress = 1.0; 

        Point delta;
        delta.x = motion.end.x - motion.start.x;
        delta.y = motion.end.y - motion.start.y;

        Point& pos = _entities[motion.entity].pos;
        pos.x = motion.start.x + delta.x * m_progress;
        pos.y = motion.start.y + delta.y * m_progress;

        prev = itr++;
    }
}

void GameScene::process(SDL_Event const& e)
{
    if(e.type == SDL_MOUSEBUTTONUP)
    {
        if(!_dragging && SDL_BUTTON_LEFT)
            _wait_input = false;
        if(!_dragging && SDL_BUTTON_RIGHT)
            _zoom = 2.5;
        _dragging = false;
    }
    if(e.type == SDL_MOUSEMOTION)
    {
        if(e.motion.state & SDL_BUTTON_LMASK)
        {
            _dragging = true;
            _camera_offset.x += e.motion.xrel;
            _camera_offset.y += e.motion.yrel;
        }
        if(e.motion.state & SDL_BUTTON_RMASK)
        {
            _dragging = true;
            _zoom += e.motion.xrel / 100.0;
            if(_zoom < 1.0)
                _zoom = 1.0;
            if(_zoom > 5.0)
                _zoom = 5.0;
        }
    }
        
}

void GameScene::process_action()
{
    if(_linestack.size() == 0)
    {
        print("Story finished.");
        return;
    }

    auto& act_itr = _linestack.top().act;
    auto act = *act_itr;
    auto& line = *(_linestack.top().line);

    act_itr++;
    if(act_itr == line.end())
        _linestack.pop();

    _wait_input = true;

    switch(act->type())
    {
        case Action::action_command:
            process_command(std::static_pointer_cast
                            <Command>(act));
            break;

        case Action::action_flag:
            set_flag(std::static_pointer_cast
                     <SetFlag>(act));
            break;

        case Action::action_condition:
            process_condition(std::static_pointer_cast
                              <Condition>(act));
            break;

        default:
            print("Not parsing action of type ",
                  act->type_name());
            break;
    }
}
