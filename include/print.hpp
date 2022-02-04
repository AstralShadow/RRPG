#ifndef ENABLE_PRINTING
#define ENABLE_PRINTING 1
#endif

#if ENABLE_PRINTING
#include <iostream>
#endif

namespace
{
#if ENABLE_PRINTING
    template<typename... T>
    [[gnu::unused]] void print(T...);

    template<typename T, typename... R>
    [[gnu::unused]] void print(T text, R... other)
    {
            std::cout << text;
            print(other...);
    }

    template<>
    [[gnu::unused]]  void print() {
        std::cout << std::endl;
    }
#else
    template<typename... T>
    [[gnu::unused]] void print(T...)
    { }
#endif
}
