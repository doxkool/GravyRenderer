#include "Time.h"

#include "Window/GLFW/glfw.h"

double d_mTime;
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

namespace Gravy
{
    double Time::GetTime()
    { 
        return glfwGetTime();
    }

    double Time::Get_Seconds() 
    {
        return d_mTime;
    }

	double Time::Get_Milliseconds()
    {
        return d_mTime * 1000;
    }

    float Time::UpdateDeltaTime()
    {
        float currentFrame = static_cast<float>(Gravy::glfw::GetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        return deltaTime;
    }

    float Time::GetDeltaTime()
    {
        return deltaTime;
    }
}