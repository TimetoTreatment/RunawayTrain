#pragma once
#include "RoadTracer.h"
#include "MotorController.h"
#include "Config.h"


class DrivingManager
{
private:

	RoadTracer* mRoadTracer;
	MotorController* mMotorController;

	DrivingManager()
	{
		mRoadTracer = RoadTracer::Instance();
		mMotorController = MotorController::Instance();
	}

	~DrivingManager()
	{
		RoadTracer::Release();
		MotorController::Release();
	}


public:

	void MainLoop()
	{
		VideoCapture cap("assets/video/roadtest3.mp4");
		Mat frame;
		bool quit;

		mMotorController->Speed('r', 72);

		for (quit = false; !quit;)
		{
			auto start = std::chrono::system_clock::now();

			cap.read(frame);

			if (frame.empty())
			{
				quit = true;
				break;
			}

			switch (mRoadTracer->Main(frame))
			{
			case Direction::Left90:
				mMotorController->Control(MotorStatus::LeftStop);
				mMotorController->Control(MotorStatus::RightForward);

				break;

			case Direction::Right90:

				mMotorController->Control(MotorStatus::RightStop);
				mMotorController->Control(MotorStatus::LeftForward);

				break;
			}

			//std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
			//int FPS = 1 / elapsed_seconds.count();
			//cout << "FPS = " << FPS << endl;

			switch (waitKey(1))
			{
			case 27:
				quit = 1;
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

