#pragma once

#include "Core/Base.h"
#include "Core/ConfigsDef.h"
#include "Core/Logger.h"
#include "Core/Time.h"

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
    void CloseWindow();
    void Shutdown();

    void NewFrame();
    void EndFrame();

    Window* GetWindowInst();
    void SwapScreenBuffer();
    void SetVsync(bool enable);
    bool* GetVsync();
    bool IsRunning();

    void SetClearColor(glm::vec4 color);
    void ClearBuffer();

    float GetDeltaTime();
}
