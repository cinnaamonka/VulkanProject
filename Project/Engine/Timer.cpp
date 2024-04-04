#include "Timer.h"

duration<float> Timer::m_ElapsedTime{};

std::chrono::high_resolution_clock::time_point Timer::m_PreviousTime{ std::chrono::high_resolution_clock::now() };
std::chrono::high_resolution_clock::time_point Timer::m_CurrentTime{};

float Timer::GetElapsed()
{
    return m_ElapsedTime.count();
}

high_resolution_clock::time_point Timer::GetCurrent()
{
    return m_CurrentTime;
}

void Timer::Update()
{
    m_CurrentTime = high_resolution_clock::now();
    m_ElapsedTime = duration<float>(m_CurrentTime - m_PreviousTime);
    m_PreviousTime = m_CurrentTime;
}
