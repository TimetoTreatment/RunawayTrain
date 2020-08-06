#pragma once


#include "opencv2/opencv.hpp"

#define PI 3.1415926

using namespace std;
using namespace cv;

Mat frame, Matrix, framePers, frameGray, frameThresh, frameEdge, frameFinal, frameFinalDuplicate;
Mat ROILane;
int LeftLanePos, RightLanePos, frameCenter, laneCenter, Result;
bool Exit;

stringstream ss;


vector<int> histrogramLane;

float xpos1 = 400;
float xpos2 = 1520;
float ypos1 = 720;
float ypos2 = 820;

Point2f Source[] = { {xpos1,ypos1}, {xpos2,ypos1}, {xpos1 - 300,ypos2}, {xpos2 + 300,ypos2} };
Point2f Destination[] = { Point2f(0,0) ,Point2f(400,0) ,Point2f(0,240) , Point2f(400,240) };

class Temp {
private:


public:
	



	static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
	{
		double dx1 = pt1.x - pt0.x;
		double dy1 = pt1.y - pt0.y;
		double dx2 = pt2.x - pt0.x;
		double dy2 = pt2.y - pt0.y;
		return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
	}

	Mat src;

	/**
	 * Helper function to display text in the center of a contour
	 */


	int Main()
	{
		//cv::Mat src = cv::imread("polygon.png");
		src = cv::imread("assets/image/circle.jpg");

		// Convert to grayscale
		cv::Mat gray;
		cv::cvtColor(src, gray, COLOR_BGR2GRAY);

		// Use Canny instead of threshold to catch squares with gradient shading
		cv::Mat bw;
		cv::Canny(gray, bw, 0, 50, 5);

		// Find contours
		std::vector<std::vector<cv::Point> > contours;
		cv::findContours(bw.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		std::vector<cv::Point> approx;
		cv::Mat dst = src.clone();

		for (int i = 0; i < contours.size(); i++)
		{
			// Approximate contour with accuracy proportional
			// to the contour perimeter
			cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.02, true);

			// Skip small or non-convex objects 
			if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
				continue;

			else if (approx.size() >= 4 && approx.size() <= 6)
			{
				// Number of vertices of polygonal curve
				int vtc = approx.size();

				// Get the cosines of all corners
				std::vector<double> cos;
				for (int j = 2; j < vtc + 1; j++)
					cos.push_back(angle(approx[j % vtc], approx[j - 2], approx[j - 1]));

				// Sort ascending the cosine values
				std::sort(cos.begin(), cos.end());

				// Get the lowest and the highest cosine
				double mincos = cos.front();
				double maxcos = cos.back();

				// Use the degrees obtained above and the number of vertices
				// to determine the shape of the contour
				if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
				{
					Rect r = cv::boundingRect(contours[i]);


					Mat extractRect = src(r);
					imshow("newshaco", extractRect);
				}
			}
		}

		cv::imshow("src", src);
		cv::imshow("dst", dst);
		cv::waitKey(0);
		return 0;
	}
};


