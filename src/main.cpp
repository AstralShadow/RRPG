#include "Game.hpp"

static Game game;

int main(int, char**)
{
    if(game.init()) return -1;

    game.run();

    return 0;
}
