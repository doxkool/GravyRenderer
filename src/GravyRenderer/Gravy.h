#include "Core/Base.h"
#include "Core/ConfigsDef.h"
#include "Core/Logger.h"

#include "Audio/Audio.h"

#include "Graphics/API/RendererAPI.h"
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

    void PlayWavAudio(const char* filename);

    void SetClearColor(glm::vec4 color);
    void ClearBuffer();
}
