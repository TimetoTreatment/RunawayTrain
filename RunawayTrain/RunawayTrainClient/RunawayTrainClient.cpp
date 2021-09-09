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
	TCP* tcp = new TCP("9510", "192.168.219.102");

	VideoCapture cap(0);

	cap.set(CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CAP_PROP_FRAME_HEIGHT, 720);
	int count = 0;

	//thread cinIntThread([&]() {
	//	for (;;)
	//		cin >> value;
	//	});

	for (;;)
	{
		Mat img;
		cap >> img;

		if (img.empty())
		{
			cout << "img.empty()" << endl;
			abort();
		}

		switch (tcp->WaitEvent(0))
		{
		case TCP::WaitEventType::MESSAGE:

			string message = tcp->ReadMsg();

			cout << message << endl;


			break;
		}

		rotate(img, img, ROTATE_90_CLOCKWISE);

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