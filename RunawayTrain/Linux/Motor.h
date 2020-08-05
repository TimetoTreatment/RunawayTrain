#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include "Config.h"

using namespace std;


class Motor
{
private:

	/* Left leg, Right motor */
	const int mLeft0 = 24;
	const int mLeft1 = 25;
	const int mLeftPwm = 23;

	/* Right leg, Left motor */
	const int mRight0 = 27;
	const int mRight1 = 28;
	const int mRightPwm = 29;


public:

	/*
		0     1     |  Output
		-------------------------
		High  Low   |  Corotation
		Low   High  |  Reversal
		Low   Low   |  Stop
	*/

	Motor()
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

	~Motor()
	{
		digitalWrite(mLeft0, LOW);
		digitalWrite(mLeft1, LOW);
		digitalWrite(mRight0, LOW);
		digitalWrite(mRight1, LOW);
	}

	void MotorControl(MotorControl leg)
	{
		switch (leg)
		{
		case MotorControl::LeftCorotation:
			digitalWrite(mLeft0, HIGH);
			digitalWrite(mLeft1, LOW);
			break;

		case MotorControl::LeftReversal:
			digitalWrite(mLeft0, LOW);
			digitalWrite(mLeft1, HIGH);
			break;

		case MotorControl::LeftStop:
			digitalWrite(mLeft0, LOW);
			digitalWrite(mLeft1, LOW);
			break;

		case MotorControl::RightCorotation:
			digitalWrite(mRight0, HIGH);
			digitalWrite(mRight1, LOW);
			break;

		case MotorControl::RightReversal:
			digitalWrite(mRight0, LOW);
			digitalWrite(mRight1, HIGH);
			break;

		case MotorControl::RightStop:
			digitalWrite(mRight0, LOW);
			digitalWrite(mRight1, LOW);
			break;
		}
	}

	void MotorSpeed(char leg, int speedPercentage)
	{

	}

	void Main()
	{
		string input;

		MotorControl(MotorControl::RightCorotation);
		MotorControl(MotorControl::LeftCorotation);

		for (int speed;;)
		{
			cin >> input;

			if (input == "exit")
				break;

			else if (input == "leftspeed")
			{
				cin >> speed;
				softPwmWrite(mLeftPwm, speed);
			}

			else if (input == "rightspeed")
			{
				cin >> speed;
				softPwmWrite(mRightPwm, speed);
			}

			if (input == "leftcorotation")
				MotorControl(MotorControl::LeftCorotation);
			else if (input == "leftreversal")
				MotorControl(MotorControl::LeftReversal);
			else if (input == "leftstop")
				MotorControl(MotorControl::LeftStop);
			else if (input == "rightcorotation")
				MotorControl(MotorControl::RightCorotation);
			else if (input == "rightreversal")
				MotorControl(MotorControl::RightReversal);
			else if (input == "rightstop")
				MotorControl(MotorControl::RightStop);
		}
	}
};