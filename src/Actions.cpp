#include "Actions.hpp"
#include <stdexcept>

Action::Action(Speech data) :
    type(action_speech),
    _data(data)
{ }

Action::Action(Action const& action) :
    type(action.type), _data() 
{
    copy_union_val(action._data);
}

void Action::operator =(Action const& action)
{
    type = action.type;
    clear_union_val();
    copy_union_val(action._data);
}

Action::~Action()
{
    clear_union_val();
}


Action::operator Speech() {
    if(type != action_speech){
        string msg = "Tried to convert action to "
                     "Speech when it's type is ";
        msg += type_name();
        throw std::runtime_error(msg);
    }
    return _data.speech;
}

Action::operator Choice() {
    if(type != action_choice){
        string msg = "Tried to convert action to "
                     "Choice when it's type is ";
        msg += type_name();
        throw std::runtime_error(msg);
    }
    return _data.choice;
}

Action::operator Flag() {
    if(type != action_flag){
        string msg = "Tried to convert action to "
                     "Flag when it's type is ";
        msg += type_name();
        throw std::runtime_error(msg);
    }
    return _data.flag;
}

Action::operator Condition() {
    if(type != action_condition){
        string msg = "Tried to convert action to "
                     "Condition when it's type is ";
        msg += type_name();
        throw std::runtime_error(msg);
    }
    return _data.condition;
}

Action::operator Command() {
    if(type != action_command){
        string msg = "Tried to convert action to "
                     "Command when it's type is ";
        msg += type_name();
        throw std::runtime_error(msg);
    }
    return _data.command;
}

string Action::type_name()
{
    switch(type)
    {
        case action_speech:
            return "Speech";

        case action_choice:
            return "Choice";
        
        case action_flag:
            return "Flag";
        
        case action_condition:
            return "Condition";
        
        case action_command:
            return "Command";

        default:
            return "Unknown";
    }
}


void Action::copy_union_val(ActionData const& data)
{
    switch(type)
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

void Action::clear_union_val()
{
    switch(type)
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
