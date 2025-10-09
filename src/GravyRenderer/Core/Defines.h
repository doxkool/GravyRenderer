#pragma once

// Stuffs required under Windoz to export classes properly
// from the shared library...
// USAGE :
//      - Add "-DBUILD_LIB" to the compiler options
//

#ifdef __WIN32__
    #ifdef BUILD_LIB
        #define LIBEX __declspec(dllexport)
    #else
        #define LIBEX __declspec(dllimport)
    #endif
#elif __linux__
    #define LIBEX       // Linux & other
    //#define LINUX_WAYLAND
    //#define LINUX_X11
#endif

#define GLM_ENABLE_EXPERIMENTAL

#define BLACK {0.0f, 0.0f, 0.0f, 1.0f}
#define WHITE {1.0f, 1.0f, 1.0f, 1.0f}
#define GRAY {0.3f, 0.3f, 0.3f, 1.0f}
#define RED {1.0f, 0.0f, 0.0f, 1.0f}
#define GREEN {0.0f, 1.0f, 0.0f, 1.0f}
#define BLUE {0.0f, 0.0f, 1.0f, 1.0f}
#define SKY_BLUE {0.31f, 0.62f, 0.90f, 1.0f}
#define ORANGE {8.0f, 0.6f, 0.1f, 1.0f}
#define DARK_ORANGE {0.5f, 0.3f, 0.1f, 1.0f}

#ifdef DEBUG
    #ifdef __WIN32__
		#define DEBUGBREAK() __debugbreak()
    #elif __LINUX__
		#include <signal.h>
		#define DEBUGBREAK() raise(SIGTRAP)
    #else
		#define DEBUGBREAK() void(0)
    #endif
#endif

#ifdef ENABLE_ASSERTS
    #define ENGINE_ASSERT(condition, ...) \
        if (!(condition)) { \
            LOG_ERROR("Assertion failed: {}", __VA_ARGS__); \
            std::terminate(); \
        }
#else
    #define ENGINE_ASSERT(condition, ...)
#endif