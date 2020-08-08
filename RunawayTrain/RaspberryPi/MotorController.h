#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include "Config.h"

using namespace std;


class MotorController
{
private:

	/* 좌측 다리, 우측 모터 */
	const int mLeft0 = 27;
	const int mLeft1 = 28;
	const int mLeftPwm = 29;

	/* 우측 다리, 좌측 모터 */
	const int mRight0 = 24;
	const int mRight1 = 25;
	const int mRightPwm = 23;

	MotorController();
	~MotorController();

public:

	/* 회전 제어 */
	void Control(MotorStatus motorStatus);
	/*
		0     1     |  Output
		-------------------------
		High  Low   |  Corotation
		Low   High  |  Reversal
		Low   Low   |  Stop
	*/

	/* 속도 조절 */
	void Speed(char side, int percentage);

	/* 테스트 */
	void Test(){}


private:
	static MotorController* sInstance;

public:
	static MotorController* Instance()
	{
		if (sInstance == nullptr)
			sInstance = new MotorController;

		return sInstance;
	}

	static void Release()
	{
		delete sInstance;
		sInstance = nullptr;
	}
};
