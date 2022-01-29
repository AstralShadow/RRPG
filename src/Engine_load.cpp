#include "Engine.hpp"

#if PRINT_LOAD_LOG
#include <iostream>
#endif

namespace
{
#if PRINT_LOAD_LOG
    template<typename... T>
    void print(T...);

    template<typename T, typename... R>
    void print(T text, R... other)
    {
            std::cout << text;
            print(other...);
    }

    template<> void print() {
        std::cout << std::endl;
    }
#else
    template<typename... R>
    void print(R...)
    { }
#endif
}

int Engine::load(std::string assets_dir)
{
    print("Loading assets from ", assets_dir);

    return 0;
}
