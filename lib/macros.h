#ifndef MACROS
#define MACROS

#ifdef __linux__
    #define PLATFORM_NAME 'L' //not windows
#elif _WIN32
    #define PLATFORM_NAME 'W'  // Windows
#else

#endif

#define MAX(x, y) (x) < (y) ? (y) : (x)

#endif
