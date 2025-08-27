#pragma once

namespace Gravy
{
    class Time
    {
    private:
        double d_mTime;
    public:
        Time(double time = 0.0);
        ~Time();

        double GetTime();
        double Get_Seconds() const { return d_mTime; }
		double Get_Milliseconds() const { return d_mTime * 1000; }
    };    
}

namespace Time
{
    float GetDeltaTime();

    float updateDeltaTime();

    double GetTime();
}