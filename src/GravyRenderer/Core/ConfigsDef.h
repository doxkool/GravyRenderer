#pragma once

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

struct GrvConfInit
{
    e_GrvApi renderingAPI = Opengl;
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