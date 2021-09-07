//#include <iostream>
//#include <wiringPi.h>
//#include <softPwm.h>
//#include "Config.h"
//#include "chrono"
//
//
//using namespace std;
//
//
//class MotorController
//{
//private:
//
//	/* 조향 모터 */
//	const int mPinSteer0 = 27;
//	const int mPinSteer1 = 28;
//	const int mPinSteerPwm = 29;
//
//	/* 주행 모터 */
//	const int mPinDrive0 = 23;
//	const int mPinDrive1 = 24;
//	const int mPinDrivePwm = 25;
//
//	MotorController();
//	~MotorController();
//
//public:
//
//	/* 회전 제어 */
//	void Control(MotorStatus motorStatus);
//	/*
//		0     1     |  Output
//		-------------------------
//		High  Low   |  Corotation
//		Low   High  |  Reversal
//		Low   Low   |  Stop
//	*/
//
//	/* 속도 조절 */
//	void Speed(char side, int percentage);
//
//	/* 테스트 */
//	void Test();
//
//
//private:
//	static MotorController* sInstance;
//
//public:
//	static MotorController* Instance()
//	{
//		if (sInstance == nullptr)
//			sInstance = new MotorController;
//
//		return sInstance;
//	}
//
//	static void Release()
//	{
//		delete sInstance;
//		sInstance = nullptr;
//	}
//};
