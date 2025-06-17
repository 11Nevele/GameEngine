#pragma once
#include <chrono>
namespace ac
{
	class Time
	{
	public:
		Time();
		void Update();
		float Delta()const;
		float RealDelta()const;
		float TimeScale() const { return timeScale; }
		float CurTime() const;
		float RealCurTime() const;
		float GetFrameRate() const;
	private:
		float timeScale;
		std::chrono::time_point<std::chrono::high_resolution_clock> preTime;
		float delta;
		float realDelta;
		float totTime;
		float realTotTime;
		

	};
}


