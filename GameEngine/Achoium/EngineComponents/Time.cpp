#include "acpch.h"
#include "Time.h"
namespace ac
{
    Time::Time()
    {
        preTime = std::chrono::high_resolution_clock::now();
        timeScale = 1.0f;
        delta = 0;
        realDelta = 0;
        totTime = 0;
        realTotTime = 0;
        
    }
    void Time::Update()
    {
        auto now = std::chrono::high_resolution_clock::now();

        // Calculate delta time (in seconds)
        realDelta = std::chrono::duration<float>(now - preTime).count();
        preTime = now;

        // Apply time scaling (e.g., 1.0 = normal, 0.5 = slow-mo)
        delta = realDelta * timeScale;

        // Update total time (unscaled and scaled)
        totTime += delta;
        realTotTime += realDelta;
        preTime = std::chrono::high_resolution_clock::now();
    }
    float Time::Delta() const
    {
        return delta;
    }
    float Time::RealDelta() const
    {
        return realDelta;
    }
    float Time::CurTime() const
    {
        return totTime;
    }
    float Time::RealCurTime() const
    {
        return realTotTime;
    }
    float Time::GetFrameRate() const
    {
        return 1.0 / realDelta;
    }
}