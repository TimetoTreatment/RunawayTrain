#pragma once
#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;


class Region
{
public:
	static bool sMousePressed;
	static int sXPosStart;
	static int sYPosStart;
	static Mat sImageInput;

	static void OnMouse(int event, int x, int y, int flags, void* userdata)
	{
		Mat img_result = sImageInput.clone();

		if (event == EVENT_LBUTTONDOWN) 
		{
			sMousePressed = true;
			sXPosStart = x;
			sYPosStart = y;
		}

		else if (event == EVENT_MOUSEMOVE) 
		{
			if (sMousePressed)
			{
				rectangle(img_result, Point(sXPosStart, sYPosStart), Point(x, y),
					Scalar(0, 255, 0), 3);

				imshow("img_color", img_result);
			}
		}

		else if (event == EVENT_LBUTTONUP)
		{
			sMousePressed = false;
			sImageInput = sImageInput(Rect(sXPosStart, sYPosStart, x - sXPosStart, y - sYPosStart));
		}
	}

	Mat Mouse(Mat image)
	{
		sImageInput = image;

		imshow("img_color", image);
		setMouseCallback("img_color", OnMouse);
		waitKey();
		destroyWindow("img_color");

		return sImageInput;
	}




	//int main(void)
	//{
	//	Region region;
	//	region.img_color = imread("shaco.jpg", IMREAD_COLOR);

	//	imshow("img_color", region.img_color);
	//	setMouseCallback("img_color", region.OnMouse);

	//	waitKey(0);

	//	return 0;
	//}
};

