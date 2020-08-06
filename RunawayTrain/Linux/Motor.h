#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include "Config.h"

using namespace std;


class Motor
{
private:

	/* 좌측 다리, 우측 모터 */
	const int mLeft0 = 24;
	const int mLeft1 = 25;
	const int mLeftPwm = 23;

	/* 우측 다리, 좌측 모터 */
	const int mRight0 = 27;
	const int mRight1 = 28;
	const int mRightPwm = 29;

public:

	Motor();
	~Motor();

	/* 회전 제어 */
	void Control(MotorControl motorControl);
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
	void Test();
};
