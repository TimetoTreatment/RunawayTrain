#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "config.h"

using namespace std;
using namespace cv;

// 1280*720@60fps -> Raspberry Pi : Camera V2

class RoadTracer
{
private:

	Mat mFrameColor, mFramePerspective, mFrameEdge, mFrameFinal, mFrameHistogram;
	Mat ROILane;
	int LeftLanePos, RightLanePos, frameCenter, laneCenter;
	bool mExit;
	vector<Vec2f> lines;
	Direction mDirection;
	vector<int> histrogramLane;
	float mXMarginTop = 267;
	float mXMarginBottom = 66;
	float mYPos = 480;
	float mYSize = 66;

	Point2f ROI[4] = {
		{mXMarginTop,mYPos}, {1280 - mXMarginTop,mYPos},
		{mXMarginBottom,mYPos + mYSize}, {1280 - mXMarginBottom,mYPos + mYSize}
	};

	Point2f Destination[4] = { Point2f(0,0) ,Point2f(400,0) ,Point2f(0,225) , Point2f(400,225) };

	void Preprocess()
	{
		warpPerspective(mFrameColor, mFramePerspective, getPerspectiveTransform(ROI, Destination), Size(400, 225));

		line(mFrameColor, ROI[0], ROI[1], Scalar(0, 0, 255), 2);
		line(mFrameColor, ROI[1], ROI[3], Scalar(0, 0, 255), 2);
		line(mFrameColor, ROI[3], ROI[2], Scalar(0, 0, 255), 2);
		line(mFrameColor, ROI[2], ROI[0], Scalar(0, 0, 255), 2);
	}

	void Threshold()
	{
		resize(mFramePerspective, mFramePerspective, { 400,225 });
		cvtColor(mFramePerspective, mFramePerspective, COLOR_BGR2GRAY);
		GaussianBlur(mFramePerspective, mFramePerspective, Size(7, 7), 0, 0);
		threshold(mFramePerspective, mFramePerspective, 127, 255, THRESH_BINARY);
		Canny(mFramePerspective, mFrameEdge, 50, 100, 3, false);
		HoughLines(mFrameEdge, lines, 1, 1 * CV_PI / 180, 100);
		cvtColor(mFrameEdge, mFrameEdge, COLOR_GRAY2BGR);

		float rho, theta;
		Point pt1, pt2;
		double cosTheta, sinTheta;
		double x0, y0;

		for (size_t i = 0; i < lines.size(); i++)
		{
			rho = lines[i][0];
			theta = lines[i][1];
			cosTheta = cos(theta);
			sinTheta = sin(theta);
			x0 = cosTheta * rho;
			y0 = sinTheta * rho;
			pt1.x = cvRound(x0 + 1000 * (-sinTheta));
			pt1.y = cvRound(y0 + 1000 * (cosTheta));
			pt2.x = cvRound(x0 - 1000 * (-sinTheta));
			pt2.y = cvRound(y0 - 1000 * (cosTheta));

			line(mFrameEdge, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
		}

		imshow("HoughLines", mFrameEdge);
		cvtColor(mFramePerspective, mFrameFinal, COLOR_GRAY2BGR);
	}

	void Histrogram()
	{
		int width = mFramePerspective.size().width;
		int height = mFramePerspective.size().height;

		mFramePerspective(Rect(0, height - 100, width, 100)).copyTo(mFrameHistogram);

		histrogramLane.resize(width);

		for (int i = 0; i < width; i++)
		{
			ROILane = mFrameHistogram(Rect(i, 0, 1, 100));
			histrogramLane[i] = (int)(sum(ROILane)[0]);
		}
	}

	void LaneFinder()
	{
		vector<int>::iterator LeftPtr;
		vector<int>::iterator RightPtr;

		for (LeftPtr = histrogramLane.begin() + 180; LeftPtr != histrogramLane.begin(); LeftPtr--)
		{
			if (*LeftPtr >= 90)
				break;
		}

		for (RightPtr = histrogramLane.begin() + 220; RightPtr != histrogramLane.end(); RightPtr++)
		{
			if (*RightPtr >= 90)
				break;
		}

		LeftLanePos = distance(histrogramLane.begin(), LeftPtr);
		RightLanePos = distance(histrogramLane.begin(), RightPtr);

		line(mFrameFinal, Point2f(LeftLanePos, 0), Point2f(LeftLanePos, 225), Scalar(0, 255, 0), 2);
		line(mFrameFinal, Point2f(RightLanePos, 0), Point2f(RightLanePos, 225), Scalar(0, 255, 0), 2);
	}

	void LaneCenter()
	{
		laneCenter = (RightLanePos - LeftLanePos) / 2 + LeftLanePos;
		frameCenter = 200;

		line(mFrameFinal, Point2f(laneCenter, 180), Point2f(laneCenter, 225), Scalar(0, 0, 255), 3);
		line(mFrameFinal, Point2f(frameCenter, 0), Point2f(frameCenter, 225), Scalar(255, 0, 0), 3);
	}


public:
	Direction Main(Mat& frame)
	{
		mDirection = Direction::Forward;

		frame.copyTo(mFrameColor);

		Preprocess();
		Threshold();
		Histrogram();
		LaneFinder();
		LaneCenter();

		namedWindow("orignal", WINDOW_NORMAL);
		imshow("orignal", mFrameColor);

		namedWindow("Perspective", WINDOW_NORMAL);
		imshow("Perspective", mFramePerspective);

		namedWindow("Final", WINDOW_NORMAL);
		imshow("Final", mFrameFinal);

		bool horizon = false;
		mExit = false;
		double theta;
		double rho;

		for (int i = 0; !mExit && i < lines.size(); i++)
		{
			theta = lines[i][1];

			if (1.3 <= theta && theta <= 1.9)
			{
				horizon = true;

				for (int j = 0; j < lines.size(); j++)
				{
					rho = lines[j][0];
					theta = lines[j][1];

					// 0.78 -> 45  
					// 1.57 -> 90
					// 2.36 -> 135

					if (!(1.3 <= theta && theta <= 1.9))
					{
						if (0.6 <= theta && theta <= 1.0)
						{
							mDirection = Direction::Right90;
						}
						else if (2.2 <= theta && theta <= 2.6)
						{
							mDirection = Direction::Left90;
						}

						mExit = true;
						break;
					}
				}
			}
		}

		if (!horizon)
		{
			int xPos = laneCenter - frameCenter;

			if (xPos < -20)
				mDirection = Direction::Left;

			else if (xPos > 20)
				mDirection = Direction::Right;

			else
				mDirection = Direction::Forward;
		}

		switch (mDirection)
		{
		case Direction::Left:
			cout << "Left\n";
			break;
		case Direction::Right:
			cout << "Right\n";
			break;
		case Direction::Left90:
			cout << "Left90\n";
			break;
		case Direction::Right90:
			cout << "Right90\n";
			break;
		case Direction::Forward:
			cout << "Forward\n";
			break;
		}

		return mDirection;
	}


private:
	static RoadTracer* sInstance;

public:
	static RoadTracer* Instance();
	static void Release();
};