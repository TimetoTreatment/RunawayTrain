#pragma once
#include "RoadTracer.h"
#include "MotorController.h"


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

			
			mRoadTracer->Main(frame);



			std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;

			int FPS = 1 / elapsed_seconds.count();
			cout << "FPS = " << FPS << endl;


			int key = waitKey(1);

			switch (key)
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

