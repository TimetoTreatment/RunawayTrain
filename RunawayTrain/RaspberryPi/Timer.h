#pragma once
#include <chrono>

using namespace std;


class Timer
{
private:
	chrono::steady_clock::time_point mStartClock;

public:
	/* �ʱ�ȭ */
	void Reset();

	/* �ɸ� �ð� ��ȯ */
	double GetElapsedTime();
};
