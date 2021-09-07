#pragma once
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Temp {
private:


public:
	int Main()
	{
		VideoCapture cap(0);

		if (!cap.isOpened()) {
			cerr << "Camera open failed!" << endl;
			return -1;
		}


		Mat frame;
		while (true) {
			cap >> frame;
			if (frame.empty())
				break;


			imshow("frame", frame);
			if (waitKey(1) == 27)
				break;
		}

		return 0;







	}
};

