#include <opencv2/opencv.hpp>
#include <iostream>
#include "Config.h"

using namespace cv;
using namespace std;

// createTrackbar("HighV", "찾을 색범위 설정", &HighV, 255);

class SignDetector
{
private:



public:



	void Main(Mat& frameOriginal, Mat& frameFinal, Direction* direction)
	{
		int LowHue = 10;
		int HighHue = 170;
		int LowSaturation = 20;
		int LowValue = 10;

		Mat img_input, img_hsv, img_binary, img_binary2;

		frameOriginal.copyTo(img_input);

		cvtColor(img_input, img_hsv, COLOR_BGR2HSV);

		inRange(img_hsv, Scalar(HighHue, LowSaturation, LowValue), Scalar(180, 255, 255), img_binary);
		inRange(img_hsv, Scalar(0, LowSaturation, LowValue), Scalar(LowHue, 255, 255), img_binary2);

		img_binary |= img_binary2;

		erode(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		dilate(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		Mat img_labels, stats, centroids;
		int numOfLables = connectedComponentsWithStats(img_binary, img_labels,
			stats, centroids, 8, CV_32S);

		int max = -1, idx = 0;
		for (int j = 1; j < numOfLables; j++) {
			int area = stats.at<int>(j, CC_STAT_AREA);
			if (max < area)
			{
				max = area;
				idx = j;
			}
		}

		if (max >= 500)
		{
			int left = stats.at<int>(idx, CC_STAT_LEFT);
			int top = stats.at<int>(idx, CC_STAT_TOP);
			int width = stats.at<int>(idx, CC_STAT_WIDTH);
			int height = stats.at<int>(idx, CC_STAT_HEIGHT);

			rectangle(frameFinal, Point(left, top), Point(left + width, top + height),
				Scalar(0, 0, 255), 1);

			cout << "SignDetector::Stop : " << max << endl;
			*direction = Direction::Stop;
		}
	}

private:
	static SignDetector* sInstance;

public:
	static SignDetector* Instance()
	{
		if (sInstance == nullptr)
			sInstance = new SignDetector;

		return sInstance;
	}

	static void Release()
	{
		delete sInstance;
		sInstance = nullptr;
	}
};