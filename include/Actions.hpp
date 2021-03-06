#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "globals.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <SDL2/SDL_rect.h>
using std::string;
using std::vector;
using std::map;
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
    string entity;
    string text; 
    virtual Type type() { return action_speech; }
};

struct Choice : public Action
{
    Choice() = default;
    string entity;
    map<string, vector<shared_ptr<Action>>> options;
    virtual Type type() { return action_choice; }
};

// Move that to Command.
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

// Or entirely split this to more Actions.
// Or make this more abstract by adding vector<> args
// Tho i do like the named args.
struct Command : public Action
{
    Command() = default;
    enum {
        STORY,
        MAP,

        SPAWN, // idle state default
        REMOVE,

        STATE, // affects sprite
        MOVE,
        LOOK,

        CAMERA,
        
        SLEEP,
        WAIT
    } command;
    
    string name;
    string state;
    SDL_Point pos;
    virtual Type type() { return action_command; }
};

#endif
