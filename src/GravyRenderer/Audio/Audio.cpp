#include "Audio/Audio.h"

namespace Gravy
{
    SoLoud::Soloud* m_soloud = nullptr;
    SoLoud::Wav testWav;

    Audio::Audio()
    {
    }
    
    Audio::~Audio()
    {
    }

    e_GrvReturnTypes Audio::Init()
    {
        m_soloud = new SoLoud::Soloud;
        auto result = m_soloud->init();

        return success;
    }

    void Audio::Shutdown()
    {
        m_soloud->deinit();
        delete m_soloud;
    }

    void Audio::PlayTest()
    {
        int loadResult = testWav.load("test.wav");
        if (loadResult != 0)
        {
            return;
        }

        m_soloud->playBackground(testWav);
        LOG_DEBUG("Playing audio...");
    }
}