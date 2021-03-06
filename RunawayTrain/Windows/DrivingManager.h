#pragma once
#include <iostream>
#include "RoadTracer.h"
#include "MotorController.h"
#include "Config.h"
#include "Timer.h"
#include "SignDetector.h"

using namespace std;


class DrivingManager
{
private:

	Machine mMachine = Machine::Windows;
	bool mDebugProcessShow = true;
	bool mDebugImageTest = true;

	RoadTracer* mRoadTracer = RoadTracer::Instance();
	MotorController* mMotorController = MotorController::Instance();
	SignDetector* mSignDetector = SignDetector::Instance();

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
		if (mMachine == Machine::Windows)
			mCapture.open("assets/video/stop.avi");

		else if (mMachine == Machine::Windows)
			mCapture.open(0);

		/* 디버그 */
		mRoadTracer->DebugMode(mDebugProcessShow, mDebugImageTest);
	}

	~DrivingManager()
	{
		RoadTracer::Release();
		MotorController::Release();
		SignDetector::Release();
	}

	void Capture()
	{
		if (!mDebugImageTest)
		{
			mCapture.read(mFrameOrigianl);

			if (mMachine == Machine::Windows)
			{
				if (mFrameOrigianl.empty())
				{
					cout << "Error : DrivingManager::Preprocess";
					exit(-1);
				}
			}
		}
		else
		{
			static int testCase = 0;

			switch (testCase)
			{
			case 0:
				mFrameOrigianl = imread("assets/image/roadtest/left.png");
				break;

			case 1:
				mFrameOrigianl = imread("assets/image/roadtest/right.png");
				break;

			case 2:
				mFrameOrigianl = imread("assets/image/roadtest/left90.png");
				break;

			case 3:
				mFrameOrigianl = imread("assets/image/roadtest/right90.png");
				break;

			case 4:
				mFrameOrigianl = imread("assets/image/roadtest/forward.png");
				break;

			case 5:
				mFrameOrigianl = imread("assets/image/roadtest/leftreal.png");
				break;

			case 6:
				mFrameOrigianl = imread("assets/image/roadtest/stop.png");
				break;
			}

			testCase++;

			if (testCase == 7)
				testCase = 0;
		}


	}

	void Preprocess()
	{
		if (mMachine == Machine::Windows)
			resize(mFrameOrigianl, mFrameOrigianl, { 640, 480 });

		imshow("Original Frame", mFrameOrigianl);

		mFrameOrigianl.copyTo(mFrameFinal);
		GaussianBlur(mFrameOrigianl, mFrameOrigianl, Size(7, 7), 0);
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

			Capture();
			Preprocess();

			mRoadTracer->Main(mFrameOrigianl, mFrameFinal, &mDirection);

			if (mStopTimer.GetElapsedTime() > 6)
				mSignDetector->Main(mFrameOrigianl, mFrameFinal, &mDirection);

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
				to_string(int(1 / mLoopTimer.GetElapsedTime())), { 560,40 },
				FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 1);

			putText(mFrameFinal,
				DirectionStr(mDirection), { 20,40 },
				FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);

			imshow("Final Frame", mFrameFinal);

			if (!mDebugImageTest)
			{
				switch (waitKey(1))
				{
				case 27:
					mExit = true;
					break;

				case ' ':
					waitKey();
					break;
				}
			}
			else
			{
				switch (waitKey())
				{
				case 27:
					mExit = true;
					break;
				}
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

