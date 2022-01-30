#include "Actions.hpp"
#include <cstring>
#include <stdexcept>

string Action::type_name()
{
    switch(type())
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



