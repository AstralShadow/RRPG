#include "Engine.hpp"

static Engine engine;

int main(int, char**)
{
    if(engine.init()) return -1;

    engine.load("assets/");

    engine.run();

    return 0;
}
