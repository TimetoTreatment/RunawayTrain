#include <wiringPi.h>
#include <iostream>

using namespace std;

enum class Control
{
	LeftCorotation,
	LeftReversal,
	LeftStop,
	RightCorotation,
	RightReversal,
	RightStop,
};

class Motor
{
private:




	const int mMotorLeft0 = 22;
	const int mMotorLeft1 = 23;
	const int mMotorRight0 = 24;
	const int mMotorRight1 = 25;

	/*
		0     1     |  Output
		-------------------------
		High  Low   |  Corotation
		Low   High  |  Reversal
		Low   Low   |  Stop
	*/




public:

	Motor()
	{
		wiringPiSetup();
		pinMode(mMotorLeft0, OUTPUT);
		pinMode(mMotorLeft1, OUTPUT);
		pinMode(mMotorRight0, OUTPUT);
		pinMode(mMotorRight1, OUTPUT);
	}

	void Control(Control control)
	{
		switch (control)
		{
		case Control::LeftCorotation:
			digitalWrite(mMotorLeft0, HIGH);
			digitalWrite(mMotorLeft1, LOW);
			break;

		case Control::LeftReversal:
			digitalWrite(mMotorLeft0, LOW);
			digitalWrite(mMotorLeft1, HIGH);
			break;
		case Control::LeftStop:
			digitalWrite(mMotorLeft0, LOW);
			digitalWrite(mMotorLeft1, LOW);
			break;
		case Control::RightCorotation:
			digitalWrite(mMotorRight0, HIGH);
			digitalWrite(mMotorRight1, LOW);
			break;

		case Control::RightReversal:
			digitalWrite(mMotorRight0, LOW);
			digitalWrite(mMotorRight1, HIGH);
			break;

		case Control::RightStop:
			digitalWrite(mMotorRight0, LOW);
			digitalWrite(mMotorRight1, LOW);
			break;
		}
	}
};


int main(void)
{
	string input;
	Control control;
	Motor motor;


	for (;;)
	{
		cin >> input;

		if (input == "leftcorotation")
			motor.Control(Control::LeftCorotation);
		else if (input == "leftreversal")
			motor.Control(Control::LeftReversal);
		else if (input == "leftstop")
			motor.Control(Control::LeftStop);
		else if (input == "rightcorotation")
			motor.Control(Control::RightCorotation);
		else if (input == "rightreversal")
			motor.Control(Control::RightReversal);
		else if (input == "rightstop")
			motor.Control(Control::RightStop);
	}

	return 0;
}