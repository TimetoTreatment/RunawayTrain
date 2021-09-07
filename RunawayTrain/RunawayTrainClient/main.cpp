#include <iostream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;



int main()
{
	VideoCapture cap(0);

	if (cap.isOpened() == false)
		return 1;

	for (;;)
	{
		Mat frame;

		cap >> frame;

		if (frame.empty())
			break;

		int sum = 0;

		for (int row = 0; row < frame.size().height; row++)
		{
			for (int col = 0; col < frame.size().width; col++)
				sum += frame.at<uchar>(row, col);
		}

		imshow("Kaka", frame);

		std::cout << sum << std::endl;

		waitKey(100);
	}






	return 0;
}