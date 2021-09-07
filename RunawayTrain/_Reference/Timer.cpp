#include "Timer.h"

void Timer::Reset()
{
	mStartClock = chrono::steady_clock::now();
}

double Timer::GetElapsedTime()
{
	chrono::duration<double> elapsed_seconds = chrono::steady_clock::now() - mStartClock;
	return elapsed_seconds.count();
}
