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
    template<typename... T>
    void print(T...)
    { }
#endif
}
