#include <iostream>
#include "Config.h"

using namespace std;


class MotorController
{
private:

	/* ���� �ٸ�, ���� ���� */
	const int mLeft0 = 24;
	const int mLeft1 = 25;
	const int mLeftPwm = 23;

	/* ���� �ٸ�, ���� ���� */
	const int mRight0 = 27;
	const int mRight1 = 28;
	const int mRightPwm = 29;

	MotorController();
	~MotorController();

public:

	/* ȸ�� ���� */
	void Control(MotorStatus motorStatus);
	/*
		0     1     |  Output
		-------------------------
		High  Low   |  Corotation
		Low   High  |  Reversal
		Low   Low   |  Stop
	*/

	/* �ӵ� ���� */
	void Speed(char side, int percentage);

	/* �׽�Ʈ */
	void Test();


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
