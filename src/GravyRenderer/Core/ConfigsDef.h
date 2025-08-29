#pragma once

#pragma region Setting Enums

enum e_GrvReturnTypes
{
    success,
    fail
};

enum e_GrvApi
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

struct GrvConfInit
{
    e_GrvApi renderingAPI = Opengl;
    int apiVersionMajor = 0;
    int apiVersionMinor = 0;
    bool apiEnableMessageCallBack = false;
    e_ApiLoggingLevel apiLoggingLevel = info;
};

struct GrvConfWindow
{
    const char* windowName  = "Gravy";
    int windowResX          = 1920;
    int windowResY          = 1080;
    e_WindowMode windowMode = windowed;
    const char* iconPath    = nullptr;
    bool windowResizable    = true;
    bool transparentFB      = false;
};

#pragma endregion Settings Structs