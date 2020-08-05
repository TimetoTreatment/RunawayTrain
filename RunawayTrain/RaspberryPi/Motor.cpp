//#include <wiringPi.h>
//#include <softPwm.h>
//#include <iostream>
//
//using namespace std;
//
//enum class Control
//{
//	LeftCorotation,
//	LeftReversal,
//	LeftStop,
//	RightCorotation,
//	RightReversal,
//	RightStop,
//};
//
//class Motor
//{
//private:
//
//
//public:
//
//	const int mMotorLeft0 = 24;
//	const int mMotorLeft1 = 25;
//	const int mMotorLeftPwm = 23;
//	const int mMotorRight0 = 27;
//	const int mMotorRight1 = 28;
//	const int mMotorRightPwm = 29;
//
//	/*
//		0     1     |  Output
//		-------------------------
//		High  Low   |  Corotation
//		Low   High  |  Reversal
//		Low   Low   |  Stop
//	*/
//
//	Motor()
//	{
//		wiringPiSetup();
//
//		pinMode(mMotorLeft0, OUTPUT);
//		pinMode(mMotorLeft1, OUTPUT);
//		pinMode(mMotorRight0, OUTPUT);
//		pinMode(mMotorRight1, OUTPUT);
//		pinMode(mMotorLeftPwm, OUTPUT);
//		pinMode(mMotorRightPwm, OUTPUT);
//		softPwmCreate(mMotorLeftPwm, 0, 100);
//		softPwmCreate(mMotorRightPwm, 0, 100);
//
//		digitalWrite(mMotorLeft0, LOW);
//		digitalWrite(mMotorLeft1, LOW);
//	}
//
//	~Motor()
//	{
//		digitalWrite(mMotorLeft0, LOW);
//		digitalWrite(mMotorLeft1, LOW);
//		digitalWrite(mMotorRight0, LOW);
//		digitalWrite(mMotorRight1, LOW);
//	}
//
//	void Control(Control control)
//	{
//		switch (control)
//		{
//		case Control::LeftCorotation:
//			digitalWrite(mMotorLeft0, HIGH);
//			digitalWrite(mMotorLeft1, LOW);
//			break;
//
//		case Control::LeftReversal:
//			digitalWrite(mMotorLeft0, LOW);
//			digitalWrite(mMotorLeft1, HIGH);
//			break;
//
//		case Control::LeftStop:
//			digitalWrite(mMotorLeft0, LOW);
//			digitalWrite(mMotorLeft1, LOW);
//			break;
//
//		case Control::RightCorotation:
//			digitalWrite(mMotorRight0, HIGH);
//			digitalWrite(mMotorRight1, LOW);
//			break;
//
//		case Control::RightReversal:
//			digitalWrite(mMotorRight0, LOW);
//			digitalWrite(mMotorRight1, HIGH);
//			break;
//
//		case Control::RightStop:
//			digitalWrite(mMotorRight0, LOW);
//			digitalWrite(mMotorRight1, LOW);
//			break;
//		}
//	}
//};
//
//
//int main(void)
//{
//	string input;
//	Control control;
//	Motor motor;
//
//	//softPwmWrite(motor.mMotorRightPwm, 100);
//	//softPwmWrite(motor.mMotorLeftPwm, 100);
//	//
//	//for (;;)
//	//{
//	//	cin >> input;
//	//
//	//	if (input == "exit")
//	//		break;
//	//
//	//	if (input == "leftcorotation")
//	//		motor.Control(Control::LeftCorotation);
//	//	else if (input == "leftreversal")
//	//		motor.Control(Control::LeftReversal);
//	//	else if (input == "leftstop")
//	//		motor.Control(Control::LeftStop);
//	//	else if (input == "rightcorotation")
//	//		motor.Control(Control::RightCorotation);
//	//	else if (input == "rightreversal")
//	//		motor.Control(Control::RightReversal);
//	//	else if (input == "rightstop")
//	//		motor.Control(Control::RightStop);
//	//}
//
//	motor.Control(Control::RightCorotation);
//	motor.Control(Control::LeftCorotation);
//
//	for (int speed = 0; speed != -1;)
//	{
//		cin >> input >> speed;
//
//		if (input == "exit")
//			break;
//
//		else if(input == "left")
//			softPwmWrite(motor.mMotorLeftPwm, speed);
//
//		else if (input == "right")
//			softPwmWrite(motor.mMotorRightPwm, speed);
//	}
//
//	return 0;
//}