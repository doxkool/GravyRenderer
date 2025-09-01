#include "Audio/Audio.h"

SoLoud::Soloud m_soloud;
SoLoud::Wav m_Wav;
e_GrvReturnTypes Audio::Init()
{
    auto result = m_soloud.init();
    if(result != 0)
    {
        return fail;
    }
    return success;
}
void Audio::Shutdown()
{
    m_soloud.deinit();
}
void Audio::CreateBus(t_AudioBusInfo* audioBusInfo)
{
    int id = m_AudioBuses.size();
    m_AudioBuses.resize(m_AudioBuses.size() + 1);
    auto &newBus = m_AudioBuses[id];
    newBus.audioBusInfo = audioBusInfo;
    LOG_DEBUG("New audio bus created.\n    ID: {}\n    Name: {}\n    Channel Mode: \n", id, newBus.audioBusInfo->name);//, newBus.audioBusInfo->channelMode);
}
int Audio::LoadAudioTrack(t_AudioTrackInfo* audioTrackInfo)
{
    int id = m_AudioTracks.size();
    m_AudioTracks.resize(m_AudioTracks.size() + 1);
    auto &newTrack = m_AudioTracks[id];
    newTrack.trackInfo = audioTrackInfo;
    auto &filePath = newTrack.trackInfo->filePath;
    
    newTrack.trackLength = newTrack.audio.getLength();
    newTrack.audio.setLooping(newTrack.trackInfo->loop);
    auto ret = newTrack.audio.load(filePath);
    if(ret != 0)
    {
        LOG_ERROR("Error: Fail to load audi track {}", filePath);
        return -1;
    }
    LOG_DEBUG("Loaded new audio track:\n    ID: {}\n    File: {}\n    Length: {}\n    Loop: {}\n", id, filePath, newTrack.trackLength, newTrack.trackInfo->loop);
    return id;
}
void Audio::PlayAudioTrack(int id)
{
    auto &audioTrack = m_AudioTracks[id];
    m_soloud.playBackground(audioTrack.audio);
    LOG_DEBUG("Playing audio:\n    ID: {}\n    File: {}\n    Length: {}\n    Loop: {}\n", id, audioTrack.trackInfo->filePath, audioTrack.trackLength, audioTrack.trackInfo->loop);
}
void Audio::PlayAudio(const char* filename, bool loop)
{
    int loadResult = m_Wav.load(filename);
    if (loadResult != 0)
    {
        LOG_ERROR("Error loading the audio file {}", filename);
        return;
    }
    if(loop)
    {
        m_Wav.setLooping(1);
    }
    m_soloud.playBackground(m_Wav);
    LOG_DEBUG("Playing audio : {}", filename);
}
void Audio::Play3dAudio(const char* filename, bool loop)
{
    LOG_ERROR("3D AUDIO NOT EMPLEMENTED YET");
}
void Audio::StopAllAudio()
{
    LOG_DEBUG("Stoping all audio...");
    m_soloud.stopAll();
}
void Audio::SetGlobalVolume(float volume)
{
    m_soloud.setGlobalVolume(volume);
}
float Audio::GetGlobalVolume()
{
    return m_soloud.getGlobalVolume();
}
