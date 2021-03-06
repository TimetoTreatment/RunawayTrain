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

	bool mDebugProcessShow = true;
	bool mDebugImageTest = true;

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
#ifdef WINDOWS
		mCapture.open("assets/video/roadtest3.mp4");
#endif
#ifdef RASPBERRYPI
		mCapture.open(0);
#endif

		/* 디버그 */
		mRoadTracer->DebugMode(mDebugProcessShow, mDebugImageTest);
	}

	~DrivingManager()
	{
		RoadTracer::Release();
		MotorController::Release();
	}

	void Preprocess()
	{
		if (!mDebugImageTest)
		{
			mCapture.read(mFrameOrigianl);

#ifdef WINDOWS
			if (mFrameOrigianl.empty())
			{
				cout << "Error : DrivingManager::Preprocess";
				exit(-1);
			}
#endif
		}
		else
		{
			static int testCase = 0;

			switch (testCase % 5)
			{
			case 0:
				mFrameOrigianl = imread("assets/image/roadtest/leftreal.png");
				break;

			case 1:
				mFrameOrigianl = imread("assets/image/roadtest/left.png");
				break;

			case 2:
				mFrameOrigianl = imread("assets/image/roadtest/right.png");
				break;

			case 3:
				mFrameOrigianl = imread("assets/image/roadtest/left90.png");
				break;

			case 4:
				mFrameOrigianl = imread("assets/image/roadtest/right90.png");
				break;
			}

			testCase++;
		}

#ifdef WINDOWS
		resize(mFrameOrigianl, mFrameOrigianl, { 640, 480 });
#endif
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

			}

			putText(mFrameFinal,
				to_string(int(1 / mLoopTimer.GetElapsedTime())), { 600,40 },
				FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 1);

			putText(mFrameFinal,
				DirectionStr(mDirection), { 20,40 },
				FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);

			namedWindow("Final", WINDOW_AUTOSIZE);
			imshow("Final", mFrameFinal);

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

