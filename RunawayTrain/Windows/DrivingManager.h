#pragma once
#include <iostream>
#include "RoadTracer.h"
#include "MotorController.h"
#include "Config.h"
#include "Timer.h"

using namespace std;


class DrivingManager
{
private:

	RoadTracer* mRoadTracer = RoadTracer::Instance();
	MotorController* mMotorController = MotorController::Instance();

	VideoCapture mCapture;
	Mat mFrameOrigianl;
	Mat mFrameFinal;

	Timer mLoopTimer;
	Timer mTurnTimer;
	Timer mStopTimer;

	Direction mDirection;

	int mLeftSpeed;
	int mRightSpeed;
	bool mExit = false;

	DrivingManager()
	{
		/* 속도 */
		mDirection = Direction::Forward;
		mLeftSpeed = 72;
		mRightSpeed = 100;

		/* 영상 */
		mCapture.open("assets/video/roadtest3.mp4");

		/* 디버그 */
		mRoadTracer->DebugMode(true, false);
	}

	~DrivingManager()
	{
		RoadTracer::Release();
		MotorController::Release();
	}

	void Preprocess()
	{
		mCapture.read(mFrameOrigianl);
		if (mFrameOrigianl.empty())
		{
			cout << "Error : DrivingManager::Preprocess";
			exit(-1);
		}

		resize(mFrameOrigianl, mFrameOrigianl, { 1280, 720 });
		mFrameOrigianl.copyTo(mFrameFinal);
	}


public:

	void MainLoop()
	{
		mStopTimer.Reset();

		mMotorController->Speed('l', mLeftSpeed);
		mMotorController->Speed('r', mRightSpeed);

		for (mExit = false; !mExit;)
		{
			mLoopTimer.Reset();
			mTurnTimer.Reset();

			Preprocess();

			mDirection = mRoadTracer->Main(mFrameOrigianl, mFrameFinal);

			//if (mStopTimer.GetElapsedTime() > 10)
			//mDirection = STOPSIGN;
			//mDirection = MARKDETECT;

			switch (mDirection)
			{
			case Direction::Forward:
				mMotorController->Control(MotorStatus::LeftForward);
				mMotorController->Control(MotorStatus::RightForward);
				mMotorController->Speed('l', mLeftSpeed);
				mMotorController->Speed('r', mRightSpeed);
				break;

			case Direction::LeftCorrection:
				mMotorController->Control(MotorStatus::LeftForward);
				mMotorController->Control(MotorStatus::RightForward);
				mMotorController->Speed('l', mLeftSpeed / 2);
				mMotorController->Speed('r', mRightSpeed);
				break;

			case Direction::RightCorrection:
				mMotorController->Control(MotorStatus::LeftForward);
				mMotorController->Control(MotorStatus::RightForward);
				mMotorController->Speed('l', mLeftSpeed);
				mMotorController->Speed('r', mRightSpeed / 2);
				break;

			case Direction::LeftTurn:
				mMotorController->Control(MotorStatus::LeftStop);
				mMotorController->Control(MotorStatus::RightForward);
				// delay(2000);
				break;

			case Direction::RightTurn:
				mMotorController->Control(MotorStatus::RightStop);
				mMotorController->Control(MotorStatus::LeftForward);
				// delay(2000);
				break;

			case Direction::Stop:
				mMotorController->Control(MotorStatus::RightStop);
				mMotorController->Control(MotorStatus::LeftStop);
				// delay(6000);
				mStopTimer.Reset();
				break;
			}

			putText(mFrameFinal,
				to_string(int(1 / mLoopTimer.GetElapsedTime())),{ 1200,40 }, 
				FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 1);

			putText(mFrameFinal,
				DirectionStr(mDirection), { 20,40 }, 
				FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);

			namedWindow("Final", WINDOW_AUTOSIZE);
			imshow("Final", mFrameFinal);

			switch (waitKey(1))
			{
			case 27:
				mExit = true;
				break;

			case ' ':
				waitKey(0);
				break;
			}
		}
	}


private:
	static DrivingManager* sInstance;

public:
	static DrivingManager* Instance()
	{
		if (sInstance == nullptr)
			sInstance = new DrivingManager;

		return sInstance;
	}

	static void Release()
	{
		delete sInstance;
		sInstance = nullptr;
	}
};

