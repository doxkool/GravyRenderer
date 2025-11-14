#pragma once

namespace Renderer
{
    class Time
    {
    private:
        
    public:
        Time() {}
        ~Time() {}

        static double GetTime();
        static double Get_Seconds();
		static double Get_Milliseconds();

        static float UpdateDeltaTime();
        static float GetDeltaTime();
    };    
}