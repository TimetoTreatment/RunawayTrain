#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include "../Utility/TCPLinux.h"
#include <unistd.h>
#include "opencv2/core/ocl.hpp"
#include "MotorManager.h"

using namespace std;
using namespace cv;


int main()
{
	TCP* tcp = new TCP("9510", "192.168.219.102");

	VideoCapture cap(0);
	MotorManager motorManager;

	cap.set(CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CAP_PROP_FRAME_HEIGHT, 720);
	int count = 0;

	//thread cinIntThread([&]() {
	//	for (;;)
	//		cin >> value;
	//	});

	motorManager.SpeedDrive(100);
	motorManager.SpeedSteer(100);

	for (bool exit = false; !exit;)
	{
		Mat img;
		cap >> img;

		rotate(img, img, ROTATE_90_CLOCKWISE);

		if (img.empty())
		{
			cout << "img.empty()" << endl;
			abort();
		}

		static bool up = false;
		static bool down = false;
		static bool left = false;
		static bool right = false;

		switch (tcp->WaitEvent(0))
		{
		case TCP::WaitEventType::MESSAGE:

			string message = tcp->ReadMsg();

			cout << message << endl;

			if (message == "EXIT")
			{
				motorManager.Control(MotorManager::Direction::STOP);
				exit = true;
				break;
			}
			else if (message == "UP_ON")
			{
				up = true;
			}
			else if (message == "UP_OFF")
			{
				up = false;

			}
			else if (message == "DOWN_ON")
			{
				down = true;
			}
			else if (message == "DOWN_OFF")
			{
				down = false;
			}
			else if (message == "LEFT_ON")
			{
				left = true;
			}
			else if (message == "LEFT_OFF")
			{
				left = false;
			}
			else if (message == "RIGHT_ON")
			{
				right = true;
			}
			else if (message == "RIGHT_OFF")
			{
				right = false;
			}

			break;
		}

		if ((up && down) || (!up && !down))
			motorManager.Control(MotorManager::Direction::STOP);
		else
		{
			if (up)
				motorManager.Control(MotorManager::Direction::FORWARD);

			if (down)
				motorManager.Control(MotorManager::Direction::BACKWORD);
		}

		if ((left && right) || (!left && !right))
			motorManager.Control(MotorManager::Direction::CENTER);
		else if (left)
			motorManager.Control(MotorManager::Direction::LEFT);

		else if (right)
			motorManager.Control(MotorManager::Direction::RIGHT);





		
		int size = img.total() * img.channels();

		tcp->SendMsg("START");

		vector<uchar> imgEncoded;

		imencode(".jpg", img, imgEncoded, { IMWRITE_JPEG_QUALITY, 70 });

		string sizeStr = to_string(imgEncoded.size());


		tcp->Send(sizeStr.c_str(), sizeStr.size() + 1);
		tcp->Send((const char*)imgEncoded.data(), imgEncoded.size());

		waitKey(1);
	}



	return 0;
}