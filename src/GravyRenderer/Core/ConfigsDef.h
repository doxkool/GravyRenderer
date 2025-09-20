#pragma once

#pragma region Setting Enums

enum e_ReturnTypes
{
    success,
    fail
};

enum e_RendererApi
{
    Opengl,
    Vulkan
};

enum e_WindowMode
{
    windowed,
    borderless,
    fullscreen
};

enum e_ApiLoggingLevel
{
    info,
    debug,
    trace
};

#pragma endregion Setting Enums
// ---------------------------------------------------------------------------
#pragma region Settings Structs

struct WindowSpec
{
    const char* windowName  = "Gravy";
    int windowResX          = 1920;
    int windowResY          = 1080;
    e_WindowMode windowMode = windowed;
    bool vsync              = true;
    const char* iconPath    = nullptr;
    bool windowResizable    = true;
    bool transparentFB      = false;
};

struct RendererSpec
{
    WindowSpec windowSpec;
    e_RendererApi renderingAPI = Opengl;
    int apiVersionMajor = 0;
    int apiVersionMinor = 0;
    bool apiEnableMessageCallBack = false;
    e_ApiLoggingLevel apiLoggingLevel = info;
};

#pragma endregion Settings Structs