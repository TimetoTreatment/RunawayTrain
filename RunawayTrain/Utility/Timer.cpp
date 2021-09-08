#include "Timer.h"

void Timer::Reset()
{
	start = std::chrono::steady_clock::now();
}


double Timer::GetElapsedTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
}
