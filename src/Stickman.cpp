#include "Stickman.hpp"
#include <SDL2/SDL_render.h>
#include <vector>
#include <cmath>
#include "Point.hpp"

using std::vector;


Stickman::Stickman(Game* engine) :
    _engine(engine),
    _pos{400, 300},
    _rotation(0),
    _body(nullptr)
{
    using std::atan2;
    auto body = new Joint();
    _body = body->self();

    const float dist = 40;

    auto left_knee =
        new Joint(_body, {atan2(2, -1), dist});
    auto right_knee =
        new Joint(_body, {atan2(2, 1), dist});

    auto left_step =
        new Joint(left_knee, {atan2(-1, 2), dist});
    auto right_step =
        new Joint(right_knee, {atan2(1, 2), dist});
    
    auto chest =
        new Joint(_body, {atan2(-1, 0), 1.2*dist});

    auto left_elbow =
        new Joint(chest, {atan2(-1 ,0), dist});
    auto right_elbow =
        new Joint(chest, {atan2(1 ,0), dist});

    auto left_hand = 
        new Joint(left_elbow, {0, dist});
    auto right_hand =
        new Joint(right_elbow, {0, dist});
}

void Stickman::tick(duration_t)
{ }


static void draw_joints_from_pos(SDL_Renderer* renderer,
                vector<shared_ptr<Joint>> joints,
                Point pos, float rotation)
{
    SDL_Point p = pos;
    SDL_Rect pos_rect{p.x - 2, p.y - 2, 4, 4};
    SDL_RenderFillRect(renderer, &pos_rect);

    for(auto joint : joints)
    {
        Point pos2(pos);
        Vector j_pos = joint->pos;
        auto rotation2(rotation + joint->pos.direction());
        j_pos.direction(rotation2);
        pos2.x += j_pos.x();
        pos2.y += j_pos.y();

        SDL_RenderDrawLine
            (renderer, pos.x, pos.y, pos2.x, pos2.y);

        draw_joints_from_pos
            (renderer, joint->children, pos2, rotation2);
    }
}

void Stickman::render(SDL_Renderer* ren)
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    draw_joints_from_pos(ren, {_body}, _pos, _rotation);
}

