#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include "../Utility/TCPLinux.h"
#include <unistd.h>

using namespace std;
using namespace cv;


int main()
{
	TCP* tcp = new TCP("9510", "192.168.219.102");

	for (;;)
	{
		tcp->WaitEvent(); // ACCEPT
		if (((string)tcp->ReadMessage()) == "ACCEPT")
			break;
	}

	VideoCapture cap(0);

	cap.set(CAP_PROP_FRAME_HEIGHT, 640);
	cap.set(CAP_PROP_FRAME_WIDTH, 360);

	for (;;)
	{
		Mat img;
		cap >> img;

		if (img.empty())
		{
			cout << "img.empty()" << endl;
			abort();
		}

		int size = img.total() * img.channels();

		tcp->Send("START", 6);
		tcp->Send((const char*)img.data, size);

		for (;;)
		{
			tcp->WaitEvent(0);
			if (((string)tcp->ReadMessage()) == "READY")
				break;
		}

		waitKey(1);
	}

	return 0;
}