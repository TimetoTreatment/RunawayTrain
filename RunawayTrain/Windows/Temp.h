#pragma once
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Temp {
private:


public:
	int Main()
	{
		Mat frame(500, 500, CV_32FC3, Scalar(0,0,255));
		int key;

		for (;;)
		{
			namedWindow("input",WINDOW_NORMAL);
			
			imshow("input", frame);

			key = waitKeyEx(0);

			cout << key << " : " << (char)key << endl;
		}










	}
};


