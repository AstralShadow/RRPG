#include "StoryParser.hpp"
#include "Actions.hpp"
#include "str_utils.hpp"

#include <memory>
#include <stdexcept>

#include "globals.hpp"
#define ENABLE_PRINTING PRINT_PARSE_LOG
#include "print.hpp"


using std::string;
using std::make_shared;
using std::stoi;

void StoryParser::
begin_story_state(string&,
                  vector<string>& args)
{
    if(args.size() < 2)
        throw std::runtime_error
            ("Can not load unnamed story.");
    if(args.size() > 2)
        throw std::runtime_error
            ("Story name can not contain spaces.");
    
    string name = args[1];

    auto& story = _stories[name];
    story.name = name;

    _target = name;
    _context_stack.empty();
    _context_stack.push(&(story.actions));
    _state = state_story;
    print("Loading story ", name);
}

void StoryParser::
begin_event_state(string&,
                  vector<string>& args)
{
    if(args.size() != 3)
        throw std::runtime_error
            ("Format: #when <storyname> <flag>");

    auto story= args[1];
    auto flag = args[2];
    
    _target = story;
    _context_stack.empty();
    _context_stack
        .push(&(_stories[story].events[flag]));
    _state = state_story;
    print("Loading event ", flag, " in ", story);
}


void StoryParser::
parse_story_command(string& line,
                    vector<string>& args)
{
    if(args[0] == "set" && args.size() == 2)
    {
        auto flag = new SetFlag;
        flag->flag = args[1];
        _context_stack.top()->emplace_back(flag);
        return;
    }

    if(args[0] == "set_map" && args.size() == 2)
    {
        auto cmd = new Command;
        cmd->command = Command::map;
        cmd->name = args[1];
        _context_stack.top()->emplace_back(cmd);
        return;
    }

    if(args[0] == "next" && args.size() == 2)
    {
        auto cmd = new Command;
        cmd->command = Command::story;
        cmd->name = args[1];
        _context_stack.top()->emplace_back(cmd);
        return;
    }

    if(args[0] == "move" && args.size() == 3)
    {
        auto cmd = new Command;
        cmd->command = Command::move;
        cmd->name = _speaker;
        cmd->pos.x = stoi(args[1]);
        cmd->pos.y = stoi(args[2]);
        _context_stack.top()->emplace_back(cmd);
        return;
    }

    if(args[0] == "choice" && args.size() > 2)
    {
        auto name = explode(' ', line, 1)[1];
        auto& action_list = _context_stack.top();
        auto last_action = action_list->back();
        if(last_action->type() == Action::action_choice)
        {
            auto last = std::static_pointer_cast
                <Choice>(last_action);
            if(last->entity == _speaker)
            {
                _context_stack.push
                    (&(last->options[name]));
                return;
            }
        }
        
        auto choice = new Choice;
        choice->entity = _speaker;

        _context_stack.top()->emplace_back(choice);
        _context_stack.push(&(choice->options[name]));
        return;
    }

    if(args[0] == "if" && args.size() == 2)
    {
        Flag flag(args[1]);
        auto block = new Condition;
        block->flag = flag;
        
        _context_stack.top()->emplace_back(block);
        _context_stack.push(&block->actions);
        return;
    }

    if(args[0] == "else" && args.size() == 1)
    {
        auto prev_list = _context_stack.top();
        _context_stack.pop();
        auto last_action = _context_stack.top()->back();
        auto last_type = last_action->type();
        if(last_type != Action::action_condition)
            throw std::runtime_error
                ("You can call :else only after :if");
        
        auto condition = std::static_pointer_cast
            <Condition>(last_action);
        if(&(condition->alternative) == prev_list)
            throw std::runtime_error
                ("Only one else block per :if allowed");

        _context_stack.push(&(condition->alternative));
        return;
    }

    if(args[0] == "end" && args.size() == 1)
    {
        _context_stack.pop();
        if(_context_stack.size() == 0)
            _state = state_none;
        return;
    }

    print("Unknown command: ", line);
}


void StoryParser::parse_story_data(string line)
{
    if(line[0] == '>')
        set_speaker(ltrim(line.substr(1)));
    else
        store_text(line);
}

void StoryParser::set_speaker(string name)
{
    if(name != "")
    {
        auto entity = _entities.find(name);
        if(entity == _entities.end())
            print("WARNING: Entity ", name,
                  " is not loaded, but used in story ",
                  _stories[_target].name);
    }
    _speaker = name;
}

void StoryParser::store_text(string text)
{
    auto speech = new Speech;
    speech->entity = _speaker;
    speech->text = text;
    _context_stack.top()->emplace_back(speech);
}
