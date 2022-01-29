#ifndef DLOBALS_HPP
#define GLOBALS_HPP

#include <chrono>

typedef std::chrono::milliseconds duration_t;
#define TICK_INTERVAL duration_t(16)

typedef uint32_t id_t;

#define WINDOW_TITLE "my window"
#define SCREEN_W 800
#define SCREEN_H 600

/* DEBUG flags */
#define PRINT_EXIT_MESSAGE 0
#define PRINT_TICK_SLEEP_TIME 0
#define PRINT_LOAD_LOG 1


#endif
