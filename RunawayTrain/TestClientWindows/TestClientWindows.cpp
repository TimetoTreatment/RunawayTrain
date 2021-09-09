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
	TCP* tcp = new TCP("9510", "192.168.219.102");

	for (;;)
	{
		tcp->WaitEvent(0);
		if (((string)tcp->ReadBuffer(7)) == "ACCEPT")
			break;
	}

	int current = 1;
	Mat img1 = imread("1.png");
	Mat img2 = imread("2.png");
	Mat img3 = imread("3.png");
	Mat img;

	for (;;)
	{
		if (current == 1)
			img = img1;
		else if (current == 2)
			img = img2;
		else if (current == 3)
			img = img3;

		current++;

		if (current == 4)
			current = 1;
		
		if (img.empty())
		{
			cout << "img.empty()" << endl;
			abort();
		}

		int size = img.total() * img.channels();

		tcp->Send("START", 6);

		tcp->Send((const char*)img.data, size);


		waitKey(1);
	}

	return 0;
}