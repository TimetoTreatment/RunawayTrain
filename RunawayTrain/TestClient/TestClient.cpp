#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include "../Utility/TCPLinux.h"
#include <unistd.h>
#include "opencv2/core/ocl.hpp"

using namespace std;
using namespace cv;


int main()
{
	int value = 70;

	thread cinIntThread([&]() {
		for (;;)
			cin >> value;
		});

	TCP* tcp = new TCP("9510", "192.168.219.102");

	VideoCapture cap(0);

	cap.set(CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CAP_PROP_FRAME_HEIGHT, 720);

	for (;;)
	{
		Mat img;
		cap >> img;

		if (img.empty())
		{
			cout << "img.empty()" << endl;
			abort();
		}

		if (img.empty())
		{
			cout << "img.empty()" << endl;
			abort();
		}

		int size = img.total() * img.channels();

		tcp->Send("START", 6);

		if (value == -1)
			break;

		vector<uchar> imgEncoded;

		imencode(".jpg", img, imgEncoded, { IMWRITE_JPEG_QUALITY, value });

		string sizeStr = to_string(imgEncoded.size());

		for (;;)
		{
			TCP::WaitEventType w = tcp->WaitEvent(0);

			if (w == TCP::WaitEventType::MESSAGE)
			{
				string str = tcp->ReadMsg();

				if (str == "READY")
					break;
			}
			else if (w == TCP::WaitEventType::DISCONNECT)
				break;
		}

		tcp->Send(sizeStr.c_str(), sizeStr.size() + 1);
		tcp->Send((const char*)imgEncoded.data(), imgEncoded.size());

		waitKey(1);
	}

	cinIntThread.detach();

	return 0;
}