#pragma once

#include "Graphics/Renderer/OpenGL.h"
#include "Graphics/Buffers.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "Graphics/Mesh.h"
#include "Graphics/Model.h"
#include "Graphics/Primitive.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

/* namespace Renderer
{
    int Init(RendererSpec* rendererSpec);
    RendererSpec* GetInitConfig();
    WindowSpec* GetWindowConfig();
    
    void CloseWindow();
    void Shutdown();

    void NewFrame();
    void EndFrame();

    Window* GetWindowInst();
    void SwapScreenBuffer();
    void SetVsync(bool enable);
    bool* GetVsync();
    glm::vec2 GetCurrentResolution();
    bool IsRunning();

    void SetClearColor(glm::vec4 color);
    void ClearBuffer();

    float GetDeltaTime();

    void SetMainCamera(Camera* cam);
    Camera* GetMainCamera();
} */
