#include "MotorController.h"

MotorController* MotorController::sInstance = nullptr;

using namespace std;
using namespace cv;


MotorController::MotorController()
{
	wiringPiSetup();

	pinMode(mLeft0, OUTPUT);
	pinMode(mLeft1, OUTPUT);
	pinMode(mRight0, OUTPUT);
	pinMode(mRight1, OUTPUT);
	pinMode(mLeftPwm, OUTPUT);
	pinMode(mRightPwm, OUTPUT);
	softPwmCreate(mLeftPwm, 0, 100);
	softPwmCreate(mRightPwm, 0, 100);

	digitalWrite(mLeft0, LOW);
	digitalWrite(mLeft1, LOW);
}

MotorController::~MotorController()
{
	digitalWrite(mLeft0, LOW);
	digitalWrite(mLeft1, LOW);
	digitalWrite(mRight0, LOW);
	digitalWrite(mRight1, LOW);
}

/*
	0     1     |  Output
	-------------------------
	High  Low   |  Corotation
	Low   High  |  Reversal
	Low   Low   |  Stop
*/

void MotorController::Control(MotorStatus motorStatus)
{
	switch (motorStatus)
	{
	case MotorStatus::LeftForward:
		digitalWrite(mLeft0, HIGH);
		digitalWrite(mLeft1, LOW);
		break;

	case MotorStatus::LeftReverse:
		digitalWrite(mLeft0, LOW);
		digitalWrite(mLeft1, HIGH);
		break;

	case MotorStatus::LeftStop:
		digitalWrite(mLeft0, LOW);
		digitalWrite(mLeft1, LOW);
		break;

	case MotorStatus::RightForward:
		digitalWrite(mRight0, LOW);
		digitalWrite(mRight1, HIGH);
		break;

	case MotorStatus::RightReverse:
		digitalWrite(mRight0, HIGH);
		digitalWrite(mRight1, LOW);
		break;

	case MotorStatus::RightStop:
		digitalWrite(mRight0, LOW);
		digitalWrite(mRight1, LOW);
		break;
	}
}


void MotorController::Speed(char side, int percentage)
{
	if (side == 'l')
		softPwmWrite(mLeftPwm, percentage);

	else if (side == 'r')
		softPwmWrite(mRightPwm, percentage);

	else
	{
		cout << "Error | Motor::MotorSpeed()" << endl;
		exit(-1);
	}
}


void MotorController::Test()
{
	bool Exit = false;

	int leftspeed = 100;
	int rightspeed = 75;

	VideoCapture cap(0);
	Mat frame;

	for (; !Exit;)
	{
		cap.read(frame);

		Speed('l', leftspeed);
		Speed('r', rightspeed);

		// 2490368 UP
		// 2621440 DOWN
		// 2424832 LEFT
		// 2555904 RIGHT

		switch (waitKey(1))
		{
		case 'w':
			Control(MotorStatus::LeftForward);
			Control(MotorStatus::RightForward);
			break;

		case 's':
			Control(MotorStatus::LeftReverse);
			Control(MotorStatus::RightReverse);
			break;

		case 'a':
			Control(MotorStatus::LeftStop);
			Control(MotorStatus::RightForward);
			break;

		case 'd':
			Control(MotorStatus::LeftForward);
			Control(MotorStatus::RightStop);
			break;

		case 'e':
			Control(MotorStatus::LeftStop);
			Control(MotorStatus::RightStop);
			break;

		case 27:
			Exit = true;
			break;
		}

		imshow("frame", frame);
	}
}
