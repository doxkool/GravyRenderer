#pragma once

#include "Core/Base.h"
#include "Core/Logger.h"
#include "Core/ConfigsDef.h"

#include "soloud.h"
#include "soloud_wav.h"

enum e_AudioTrackState
{
    Playing,
    Paused,
    Stoped,
    Waiting
};

struct t_AudioTrackInfo
{
    const char* filePath = "";
    bool loop = false;
    glm::vec2 startEndPoint = {0.0f, -1.0f}; // Define the set point to start and end the track use to crop the audio tack.
    int audioBus = 0;
};

struct t_AudioTrack
{
    const char* name = "track";
    t_AudioTrackInfo* trackInfo;
    SoLoud::Wav audio;
    float volume = 1.0;
    double trackLength = 0.0;
    e_AudioTrackState state;
};

struct t_GlobalAudioState
{
    float volume = 1.0f;
};

namespace Gravy
{
    class Audio
    {
    private:
        
    public:
        Audio() {}
        ~Audio() {}

        e_GrvReturnTypes Init();
        void Shutdown();

        int LoadAudioTrack(t_AudioTrackInfo* audioTrackInfo);
        
        void PlayAudioTrack(int id);
        void PlayAudio(const char* filename, bool loop = false);
        void Play3dAudio(const char* filename, bool loop = false);

        void StopAllAudio();

        void SetGlobalVolume(float volume);
        float GetGlobalVolume();

        t_GlobalAudioState m_GlobalAudioState;
        std::vector<t_AudioTrack> m_AudioTracks;
    };
}
