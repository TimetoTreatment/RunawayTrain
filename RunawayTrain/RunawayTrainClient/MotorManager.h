#pragma once
#include <wiringPi.h>
#include <softPwm.h>


class MotorManager
{
public:

	enum class Direction
	{
		STOP,
		FORWARD,
		BACKWORD,
		LEFT,
		RIGHT,
	};

	/* 회전 제어 */
	void Control(Direction direction)
	{
		switch (direction)
		{
		case Direction::STOP:
			digitalWrite(pinDrive0, LOW);
			digitalWrite(pinDrive1, LOW);
			break;

		case Direction::FORWARD:
			digitalWrite(pinDrive0, HIGH);
			digitalWrite(pinDrive1, LOW);
			break;

		case Direction::BACKWORD:
			digitalWrite(pinDrive0, LOW);
			digitalWrite(pinDrive1, HIGH);
			break;

		case Direction::LEFT:
			digitalWrite(pinSteer0, LOW);
			digitalWrite(pinSteer1, HIGH);
			break;

		case Direction::RIGHT:
			digitalWrite(pinSteer0, HIGH);
			digitalWrite(pinSteer1, LOW);
			break;
		}
	}

	/*
		0     1     |  Output
		-------------------------
		High  Low   |  Corotation
		Low   High  |  Reversal
		Low   Low   |  Stop
	*/

	/* 속도 조절 */
	void SpeedSteer(int percentage)
	{
		softPwmWrite(pinDrivePwm, percentage);
	}

	void SpeedDrive(int percentage)
	{
		softPwmWrite(pinSteerPwm, percentage);
	}

	MotorManager()
	{
		wiringPiSetup();

		pinMode(pinSteer0, OUTPUT);
		pinMode(pinSteer1, OUTPUT);
		pinMode(pinSteerPwm, OUTPUT);

		pinMode(pinDrive0, OUTPUT);
		pinMode(pinDrive1, OUTPUT);
		pinMode(pinDrivePwm, OUTPUT);

		softPwmCreate(pinSteerPwm, 0, 100);
		softPwmCreate(pinDrivePwm, 0, 100);

		digitalWrite(pinSteer0, LOW);
		digitalWrite(pinSteer1, LOW);

		digitalWrite(pinDrive0, LOW);
		digitalWrite(pinDrive1, LOW);
	}


	~MotorManager()
	{
		digitalWrite(pinSteer0, LOW);
		digitalWrite(pinSteer1, LOW);
		digitalWrite(pinDrive0, LOW);
		digitalWrite(pinDrive1, LOW);
	}


private:

	/* 조향 모터 */
	const int pinSteer0 = 27;
	const int pinSteer1 = 28;
	const int pinSteerPwm = 29;

	/* 주행 모터 */
	const int pinDrive0 = 23;
	const int pinDrive1 = 24;
	const int pinDrivePwm = 25;
};

