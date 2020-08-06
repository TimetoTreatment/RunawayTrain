#include "Motor.h"

using namespace std;





Motor::Motor()
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

Motor::~Motor()
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

void Motor::Control(MotorControl motorControl)
{
	switch (motorControl)
	{
	case MotorControl::LeftForward:
		digitalWrite(mLeft0, HIGH);
		digitalWrite(mLeft1, LOW);
		break;

	case MotorControl::LeftReverse:
		digitalWrite(mLeft0, LOW);
		digitalWrite(mLeft1, HIGH);
		break;

	case MotorControl::LeftStop:
		digitalWrite(mLeft0, LOW);
		digitalWrite(mLeft1, LOW);
		break;

	case MotorControl::RightForward:
		digitalWrite(mRight0, HIGH);
		digitalWrite(mRight1, LOW);
		break;

	case MotorControl::RightReverse:
		digitalWrite(mRight0, LOW);
		digitalWrite(mRight1, HIGH);
		break;

	case MotorControl::RightStop:
		digitalWrite(mRight0, LOW);
		digitalWrite(mRight1, LOW);
		break;
	}
}


void Motor::Speed(char side, int percentage)
{
	if (side == 'L')
		softPwmWrite(mLeftPwm, percentage);

	else if (side == 'R')
		softPwmWrite(mRightPwm, percentage);

	else
	{
		cout << "Error | Motor::MotorSpeed()" << endl;
		exit(-1);
	}
}


void Motor::Test()
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
			Control(MotorControl::LeftForward);
		else if (userControl == "lr" || userControl == "leftreverse")
			Control(MotorControl::LeftReverse);
		else if (userControl == "ls" || userControl == "leftstop")
			Control(MotorControl::LeftStop);
		else if (userControl == "rf" || userControl == "rightforward")
			Control(MotorControl::RightForward);
		else if (userControl == "rr" || userControl == "rightreverse")
			Control(MotorControl::RightReverse);
		else if (userControl == "rs" || userControl == "rightstop")
			Control(MotorControl::RightStop);
	}
}