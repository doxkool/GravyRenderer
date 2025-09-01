#pragma once

#include "Core/Base.h"
#include "Core/Logger.h"
#include "Core/ConfigsDef.h"

#include "soloud.h"
#include "soloud_wav.h"

enum e_AudioTrackState
{
    Playing = 0,
    Paused,
    Stoped,
    Waiting
};

enum e_AudioChannelMode
{
    Mono = 0,
    Stereo
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

struct t_AudioBusInfo
{
    const char* name = "bus";
    float volume = 1.0;
    e_AudioChannelMode channelMode = Stereo;
};

struct t_AudioBus
{
    t_AudioBusInfo* audioBusInfo;
};

struct t_GlobalAudioState
{
    float volume = 1.0f;
};

class Audio
{
private:
    
public:
    Audio() {}
    ~Audio() {}
    e_GrvReturnTypes Init();
    void Shutdown();
    void CreateBus(t_AudioBusInfo* audioBusInfo);
    int LoadAudioTrack(t_AudioTrackInfo* audioTrackInfo);
    
    void PlayAudioTrack(int id);
    void PlayAudio(const char* filename, bool loop = false);
    void Play3dAudio(const char* filename, bool loop = false);
    void StopAllAudio();
    void SetGlobalVolume(float volume);
    float GetGlobalVolume();
    t_GlobalAudioState m_GlobalAudioState;
    std::vector<t_AudioTrack> m_AudioTracks;
    std::vector<t_AudioBus> m_AudioBuses;
};
