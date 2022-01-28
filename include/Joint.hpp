#ifndef JOINT_HPP
#define JOINT_HPP

#include "Vector.hpp"
#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;


struct Joint
{
    shared_ptr<Joint> parent;
    /** Base position relative to the parent */
    Vector base_pos;
    /** How much it can change from the base pos. */
    Vector margin;
    Vector pos;
    vector<shared_ptr<Joint>> children;
    shared_ptr<Joint> _self = nullptr;


    Joint(Joint* _parent, Vector _pos,
                          Vector _margin = {0, 0}) :
        Joint(_parent->self(), _pos, _margin) {}

    Joint(shared_ptr<Joint> _parent, Vector _pos,
                            Vector _margin = {0, 0}) :
        parent(_parent),
        base_pos(_pos),
        margin(_margin),
        pos(_pos),
        children()
    {
        _parent->children.push_back(self());
    }

    Joint() :
        parent(nullptr),
        base_pos(0, 0),
        margin(0 ,0),
        pos(0, 0),
        children()
    { }


    shared_ptr<Joint> self()
    {
        if(_self) return _self;
        _self = shared_ptr<Joint>(this);
        return _self;
    }

    void add(Joint* child, Vector pos,
             Vector margin = {0, 0})
    {
        add(child->self(), pos, margin);
    }

    void add(shared_ptr<Joint> child,
             Vector pos, Vector margin = {0, 0})
    {
        children.push_back(child);
        child->base_pos = pos;
        child->pos = pos;
        child->margin = margin;
        child->parent = self();
    }


    bool cleaning = false;
    void clear()
    {
        if(cleaning) return;
        cleaning = true;

        for(auto child : children)
            child->clear();
        children.empty();
    }
};

#endif
