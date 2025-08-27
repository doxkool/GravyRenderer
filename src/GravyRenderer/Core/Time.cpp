#include "Time.h"

#include "Window/GLFW/glfw.h"

namespace Gravy
{
    Time::Time(double time)
        : d_mTime(time)
    {
    }
    
    Time::~Time()
    {
    }

    double Time::GetTime()
    { 
        return glfwGetTime();
    }
}

namespace Time
{
    float deltaTime = 0.0f; // time between current frame and last frame
    float lastFrame = 0.0f;

    float updateDeltaTime()
    {
        float currentFrame = static_cast<float>(Gravy::glfw::GetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        return deltaTime;
    }

    double GetTime()
    {
        return Gravy::glfw::GetTime();
    }

    float GetDeltaTime()
    {
        return deltaTime;
    }
}