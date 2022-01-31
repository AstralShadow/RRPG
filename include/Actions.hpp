#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "globals.hpp"
#include <vector>
#include <string>
#include <memory>
#include <SDL2/SDL_rect.h>
using std::string;
using std::vector;
using std::shared_ptr;

typedef string Flag;


class Action
{
public:
    enum Type
    {
        action_speech,
        action_choice,
        action_flag,
        action_condition,
        action_command
    };

    virtual ~Action() = default;

    virtual Type type() = 0;

    string type_name();

protected:
    Action() = default;
};

struct Speech : public Action
{
    Speech() = default;
    string character;
    string text; 
    virtual Type type() { return action_speech; }
};

struct Choice : public Action
{
    Choice() = default;
    string character;
    std::pair<string, vector<shared_ptr<Action>>> options;
    virtual Type type() { return action_choice; }
};

struct SetFlag : public Action
{
    SetFlag() = default;
    Flag flag;

    virtual Type type() { return action_flag; }
};

struct Condition : public Action
{
    Condition() = default;
    Flag flag;
    vector<shared_ptr<Action>> actions;
    vector<shared_ptr<Action>> alternative;
    virtual Type type() { return action_condition; }
};

struct Command : public Action
{
    Command() = default;
    enum {
        map,
        move,
        spawn,
        despawn
    } command;
    
    string name;
    SDL_Point pos;
    virtual Type type() { return action_command; }
};

#endif
