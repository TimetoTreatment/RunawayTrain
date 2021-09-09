#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include "../Utility/TCP.h"

using namespace std;
using namespace cv;


int main()
{
	int value = 90;

	thread cinIntThread([&]() {
		for (;;)
			cin >> value;
		});



	TCP* tcp = new TCP("9510", "192.168.219.102");


	int current = 1;
	Mat img1 = imread("1.png");
	Mat img2 = imread("2.png");
	Mat img3 = imread("3.png");
	Mat img;

	for (;;)
	{
		if (current == 1)
			img = img1;
		//else if (current == 2)
		//	img = img2;
		//else if (current == 3)
		//	img = img3;

		//current++;

		if (current == 4)
			current = 1;

		if (img.empty())
		{
			cout << "img.empty()" << endl;
			abort();
		}


		int size = img.total() * img.channels();

		tcp->Send("START", 6);

		std::vector<uchar> buff;//buffer for coding
		std::vector<int> param(2);
		param[0] = cv::IMWRITE_JPEG_QUALITY;

		param[1] = value;//default(95) 0-100
		cv::imencode(".jpg", img, buff, param);

		string sizeStr = to_string(buff.size());

		tcp->Send(sizeStr.c_str(), sizeStr.size() + 1);
		tcp->Send((const char*)buff.data(), buff.size());

		waitKey(1);
	}

	return 0;
}