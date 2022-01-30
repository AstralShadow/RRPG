#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include "globals.hpp"
#include <vector>
#include <string>
#include <SDL2/SDL_rect.h>
using std::string;
using std::vector;


typedef string Flag;
class Action;


struct Speech
{
    string character;
    string text; 
};

struct Choice
{
    string character;
    std::pair<string, vector<Action>> options;
};

struct Condition
{
    Flag trigger;
    vector<Action> actions;
};

struct Command
{
    enum {
        map,
        move,
        spawn
    } type;
    
    string name;
    SDL_Point pos;
};

class Action
{
public: 
    Action(Speech data) :
        _data(data), _state(action_speech) { }

    Action(Action const& action) :
        _data(), _state(action._state)
    {
        copy_union_val(action._data);
    }

    void operator =(Action const& action)
    {
        _state = action._state;
        clear_union_val();
        copy_union_val(action._data);
    }

    ~Action()
    {
        clear_union_val();
    }

private:
    union ActionData{
        ActionData(){}
        ActionData(Speech data) : speech(data) {}
        ActionData(Choice data) : choice(data) {}
        ActionData(Flag data) : flag(data) {}
        ActionData(Condition data) : condition(data) {}
        ActionData(Command data) : command(data) {}
        ~ActionData(){}

        Speech speech;
        Choice choice;
        Flag flag;
        Condition condition;
        Command command;
    } _data;

    enum {
        action_speech,
        action_choice,
        action_flag,
        action_condition,
        action_command
    } _state;

    void clear_union_val()
    {
        switch(_state)
        {
            case action_speech:
                _data.speech.~Speech();
                break;
            case action_choice:
                _data.choice.~Choice();
                break;
            case action_flag:
                _data.flag.~string();
                break;
            case action_condition:
                _data.condition.~Condition();
                break;
            case action_command:
                _data.command.~Command();
                break;
        }
    }

    void copy_union_val(ActionData const& data)
    {
        switch(_state)
        {
            case action_speech:
                _data.speech = data.speech;
                break;
            case action_choice:
                _data.choice = data.choice;
                break;
            case action_flag:
                _data.flag = data.flag;
                break;
            case action_condition:
                _data.condition = data.condition;
                break;
            case action_command:
                _data.command = data.command;
                break;
        }
    }
};



#endif
