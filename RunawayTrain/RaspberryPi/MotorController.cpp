#include "MotorController.h"

MotorController* MotorController::sInstance = nullptr;

using namespace std;
using namespace cv;


MotorController::MotorController()
{
	wiringPiSetup();

	pinMode(mPinSteer0, OUTPUT);
	pinMode(mPinSteer1, OUTPUT);
	pinMode(mPinSteerPwm, OUTPUT);

	pinMode(mPinDrive0, OUTPUT);
	pinMode(mPinDrive1, OUTPUT);
	pinMode(mPinDrivePwm, OUTPUT);

	softPwmCreate(mPinSteerPwm, 0, 100);
	softPwmCreate(mPinDrivePwm, 0, 100);

	digitalWrite(mPinSteer0, LOW);
	digitalWrite(mPinSteer1, LOW);

	digitalWrite(mPinDrive0, LOW);
	digitalWrite(mPinDrive1, LOW);
}

MotorController::~MotorController()
{
	digitalWrite(mPinSteer0, LOW);
	digitalWrite(mPinSteer1, LOW);
	digitalWrite(mPinDrive0, LOW);
	digitalWrite(mPinDrive1, LOW);
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
	case MotorStatus::Forward:
		digitalWrite(mPinDrive0, HIGH);
		digitalWrite(mPinDrive1, LOW);
		break;

	case MotorStatus::Reverse:
		digitalWrite(mPinDrive0, LOW);
		digitalWrite(mPinDrive1, HIGH);
		break;

	case MotorStatus::Stop:
		digitalWrite(mPinDrive0, LOW);
		digitalWrite(mPinDrive1, LOW);
		break;

	case MotorStatus::Left:
		digitalWrite(mPinSteer0, LOW);
		digitalWrite(mPinSteer1, HIGH);
		break;

	case MotorStatus::Right:
		digitalWrite(mPinSteer0, HIGH);
		digitalWrite(mPinSteer1, LOW);
		break;

	case MotorStatus::Center:
		digitalWrite(mPinSteer0, LOW);
		digitalWrite(mPinSteer1, LOW);
		break;
	}
}


void MotorController::Speed(char side, int percentage)
{
	if (side == 's')
		softPwmWrite(mPinSteerPwm, percentage);

	else if (side == 'd')
		softPwmWrite(mPinDrivePwm, percentage);

	else
	{
		cout << "Error | Motor::MotorSpeed()" << endl;
		exit(-1);
	}
}


void MotorController::Test()
{
	bool Exit = false;

	int steerSpeed = 70;
	int driveSpeed = 100;

	VideoCapture cap(0);
	Mat frame;

	cap.set(CAP_PROP_BRIGHTNESS, 50);

	Speed('s', steerSpeed);
	Speed('d', driveSpeed);

	int n = 0;

	for (; !Exit;n++)
	{
		cap.read(frame);
		rotate(frame, frame, ROTATE_90_CLOCKWISE);

		// 2490368 UP
		// 2621440 DOWN
		// 2424832 LEFT
		// 2555904 RIGHT

		switch (waitKey(1))
		{
		case 'w':
			Control(MotorStatus::Forward);
			break;

		case 's':
			Control(MotorStatus::Reverse);
			break;

		case 'a':
			Control(MotorStatus::Left);
			break;

		case 'd':
			Control(MotorStatus::Right);
			break;

		case ' ':
			Control(MotorStatus::Stop);
			break;

		case 'c':
			Control(MotorStatus::Center);
			break;

		case 27:
			Exit = true;
			break;
		}

		imshow("frame", frame);

		cout << "frame : " << n << endl;
	}
}
