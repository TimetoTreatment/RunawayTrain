//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Timer
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/*
	1. Features
		- Get elapsed time

	2. How to use
		- Reset() : Set point
		- GetElapsedTime() : Get elapsed time in milliseconds
*/

#pragma once
#include <chrono>


class Timer
{
private:
	std::chrono::steady_clock::time_point start;

public:
	/* �ʱ�ȭ */
	void Reset();

	/* �ɸ� �ð� ��ȯ */
	double GetElapsedTime();
};
