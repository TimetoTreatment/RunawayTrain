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

	RoadTracer* mRoadTracer;
	MotorController* mMotorController;

	VideoCapture mCapture;
	Mat mFrameColor;

	Timer mLoopTimer;
	Timer mTurnTimer;
	Timer mStopTimer;

	Direction mDirection;

	int mLeftSpeed = 72;
	int mRightSpeed = 100;

	bool mExit = false;


	DrivingManager()
	{
		mRoadTracer = RoadTracer::Instance();
		mMotorController = MotorController::Instance();

		mCapture.open("assets/video/roadtest3.mp4");
	}

	~DrivingManager()
	{
		RoadTracer::Release();
		MotorController::Release();
	}

	void Preprocess()
	{
		mCapture.read(mFrameColor);
		if (mFrameColor.empty())
		{
			mExit = true;
			exit(-1);
		}

		resize(mFrameColor, mFrameColor, { 1280, 720 });
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

			mDirection = mRoadTracer->Main(mFrameColor);

			//if (mStopTimer.GetElapsedTime() > 10)
			//mDirection = STOPSIGN;
			//mDirection = MARKDETECT;

			switch (mDirection)
			{
			case Direction::Left:
				mMotorController->Control(MotorStatus::LeftForward);
				mMotorController->Control(MotorStatus::RightForward);
				mMotorController->Speed('l', mLeftSpeed / 2);
				mMotorController->Speed('r', mRightSpeed);
				break;

			case Direction::Right:
				mMotorController->Control(MotorStatus::LeftForward);
				mMotorController->Control(MotorStatus::RightForward);
				mMotorController->Speed('l', mLeftSpeed);
				mMotorController->Speed('r', mRightSpeed / 2);
				break;

			case Direction::Left90:
				mMotorController->Control(MotorStatus::LeftStop);
				mMotorController->Control(MotorStatus::RightForward);
				// delay(2000);
				break;

			case Direction::Right90:
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

			cout << (int)(1 / mLoopTimer.GetElapsedTime()) << "\n";

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

