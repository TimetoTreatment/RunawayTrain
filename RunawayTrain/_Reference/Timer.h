#pragma once
#include <chrono>

using namespace std;


class Timer
{
private:
	chrono::steady_clock::time_point mStartClock;

public:
	/* 초기화 */
	void Reset();

	/* 걸린 시간 반환 */
	double GetElapsedTime();
};
