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
    Flag flag;
    vector<Action> actions;
    vector<Action> alternative;
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
    enum {
        action_speech,
        action_choice,
        action_flag,
        action_condition,
        action_command
    } type;

    Action(Speech data);

    Action(Action const& action);

    void operator =(Action const& action);

    ~Action();

    operator Speech();
    operator Choice();
    operator Flag();
    operator Condition();
    operator Command();

    string type_name();

private:
    union ActionData
    {
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


    void copy_union_val(ActionData const& data);
    void clear_union_val();

};

#endif
