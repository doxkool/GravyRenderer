#include "Core/Base.h"
#include "Core/ConfigsDef.h"
#include "Core/Logger.h"
#include "Core/Time.h"

#include "Audio/Audio.h"

#include "Graphics/API/RendererAPI.h"
#include "Graphics/Buffers.h"
#include "Graphics/Camera.h"
#include "Graphics/Lights.h"
#include "Graphics/Material.h"
#include "Graphics/Mesh.h"
#include "Graphics/Model.h"
#include "Graphics/Primitive.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include "Window/Window.h"

#include "Core/Input.h"

namespace Gravy
{
    e_GrvReturnTypes Init(GrvConfInit* confInit, GrvConfWindow* confWindow);
    void Shutdown();

    void NewFrame();
    void EndFrame();

    Window* GetWindowInst();
    void SwapScreenBuffer();
    bool IsRunning();

    int LoadAudioTrack(t_AudioTrackInfo* audioTrackInfo);
    void PlayAudioTrack(int id);
    void PlayAudio(const char* filename, bool loop = false);
    void StopAllAudio();

    void SetClearColor(glm::vec4 color);
    void ClearBuffer();

    float GetDeltaTime() { return ::Time::GetDeltaTime(); }
}
