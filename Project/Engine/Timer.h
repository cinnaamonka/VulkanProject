#pragma once
#include <chrono>
using namespace std::chrono;

class Timer final 
{
public:
	Timer() = default;
	static float GetElapsed();
	static high_resolution_clock::time_point GetCurrent();
	static void Update();
private:
	
	static duration<float> m_ElapsedTime;
	static high_resolution_clock::time_point m_PreviousTime;
	static high_resolution_clock::time_point m_CurrentTime;
};

