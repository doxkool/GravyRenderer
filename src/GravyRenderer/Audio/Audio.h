#pragma once

#include "Core/Logger.h"
#include "Core/ConfigsDef.h"

#include "soloud.h"
#include "soloud_wav.h"

namespace Gravy
{
    class Audio
    {
    private:
        
    public:
        Audio();
        ~Audio();

        e_GrvReturnTypes Init();
        void Shutdown();

        void PlayWav(const char* filename);
    };
}
