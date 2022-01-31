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
#define PRINT_PARSE_LOG 1
#define PRINT_PARSE_READFILE_LOG 0
#define LOAD_TEXTURES_CACHE 0
using std::chrono::milliseconds;
//#define DELAY_LOAD_TIME milliseconds(100)

// 32 - Loading Screen
#define PRELOAD_FONT_SIZES {18, 28, 32, 36}
#define FONT_NAME "comic_shanns_2.ttf"

#endif
