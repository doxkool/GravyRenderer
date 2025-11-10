#pragma once

#ifdef ENABLE_ENGINE_TRACING
    #include <tracy/Tracy.hpp>
    #include <tracy/TracyOpenGL.hpp>
#else
    #define ZoneNamedN(...)     void(0)
    #define ZoneScoped          void(0)
    #define ZoneScopedN(...)    void(0)
    #define ZoneText(...)       void(0)
    #define ZoneValue(...)      void(0)
#endif