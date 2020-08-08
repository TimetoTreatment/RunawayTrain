#pragma once
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Temp {
private:


public:
	int Main()
	{
		double rho, theta;
		Point pt1, pt2;
		double cosTheta, sinTheta;
		double x0, y0;
		Mat edge = imread("assets/image/roadtest3.png");

		theta = 0;

		for (;;)
		{
			cout << theta << endl;

			rho = 200;
			cosTheta = cos(theta);
			sinTheta = sin(theta);
			x0 = cosTheta * rho;
			y0 = sinTheta * rho;
			pt1.x = cvRound(x0 + 1000 * (-sinTheta));
			pt1.y = cvRound(y0 + 1000 * (cosTheta));
			pt2.x = cvRound(x0 - 1000 * (-sinTheta));
			pt2.y = cvRound(y0 - 1000 * (cosTheta));

			line(edge, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);

			theta = (theta + 0.05);


			imshow("asdf", edge);
			waitKey(0);
		}










	}
};


