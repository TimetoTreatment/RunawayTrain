#include "MotorController.h"

MotorController* MotorController::sInstance = nullptr;

using namespace std;





MotorController::MotorController()
{
	//wiringPiSetup();

	//pinMode(mLeft0, OUTPUT);
	//pinMode(mLeft1, OUTPUT);
	//pinMode(mRight0, OUTPUT);
	//pinMode(mRight1, OUTPUT);
	//pinMode(mLeftPwm, OUTPUT);
	//pinMode(mRightPwm, OUTPUT);
	//softPwmCreate(mLeftPwm, 0, 100);
	//softPwmCreate(mRightPwm, 0, 100);

	//digitalWrite(mLeft0, LOW);
	//digitalWrite(mLeft1, LOW);
}

MotorController::~MotorController()
{
	//digitalWrite(mLeft0, LOW);
	//digitalWrite(mLeft1, LOW);
	//digitalWrite(mRight0, LOW);
	//digitalWrite(mRight1, LOW);
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
	//switch (motorStatus)
	//{
	//case MotorStatus::LeftForward:
	//	digitalWrite(mLeft0, HIGH);
	//	digitalWrite(mLeft1, LOW);
	//	break;

	//case MotorStatus::LeftReverse:
	//	digitalWrite(mLeft0, LOW);
	//	digitalWrite(mLeft1, HIGH);
	//	break;

	//case MotorStatus::LeftStop:
	//	digitalWrite(mLeft0, LOW);
	//	digitalWrite(mLeft1, LOW);
	//	break;

	//case MotorStatus::RightForward:
	//	digitalWrite(mRight0, LOW);
	//	digitalWrite(mRight1, HIGH);
	//	break;

	//case MotorStatus::RightReverse:
	//	digitalWrite(mRight0, HIGH);
	//	digitalWrite(mRight1, LOW);
	//	break;

	//case MotorStatus::RightStop:
	//	digitalWrite(mRight0, LOW);
	//	digitalWrite(mRight1, LOW);
	//	break;
	//}
}


void MotorController::Speed(char side, int percentage)
{
	//if (side == 'l')
	//	softPwmWrite(mLeftPwm, percentage);

	//else if (side == 'r')
	//	softPwmWrite(mRightPwm, percentage);

	//else
	//{
	//	cout << "Error | Motor::MotorSpeed()" << endl;
	//	exit(-1);
	//}
}


void MotorController::Test()
{
	int speed;
	string userControl;

	for (;;)
	{
		cin >> userControl;

		if (userControl == "exit")
			break;

		else if (userControl == "leftspeed")
		{
			cin >> speed;
			Speed('l', speed);
		}

		else if (userControl == "rightspeed")
		{
			cin >> speed;
			Speed('r', speed);
		}

		else if (userControl == "lf" || userControl == "leftforward")
			Control(MotorStatus::LeftForward);
		else if (userControl == "lr" || userControl == "leftreverse")
			Control(MotorStatus::LeftReverse);
		else if (userControl == "ls" || userControl == "leftstop")
			Control(MotorStatus::LeftStop);
		else if (userControl == "rf" || userControl == "rightforward")
			Control(MotorStatus::RightForward);
		else if (userControl == "rr" || userControl == "rightreverse")
			Control(MotorStatus::RightReverse);
		else if (userControl == "rs" || userControl == "rightstop")
			Control(MotorStatus::RightStop);
	}
}