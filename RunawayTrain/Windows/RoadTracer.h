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

	float mXMarginTop = 267;
	float mXMarginBottom = 66;
	float mYPos = 480;
	float mYSize = 66;

	Point2f ROI[4] = {
		{mXMarginTop,mYPos}, {1280 - mXMarginTop,mYPos},
		{mXMarginBottom,mYPos + mYSize}, {1280 - mXMarginBottom,mYPos + mYSize}
	};

	vector<Vec2f> mLines;
	vector<int> mHistrogramLane;
	int mLeftLanePos, mRightLanePos, mFrameCenter, mLaneCenter;

	Direction mDirection;
	bool mExit;





	Point2f Destination[4] = {
		Point2f(0,0) ,
		Point2f(400,0) ,
		Point2f(0,225) ,
		Point2f(400,225)
	};

	void Preprocess();

	void Test(bool enable = false);

	void Threshold();

	void Histrogram();

	void LaneFinder();

	void LaneCenter();

	void Show(bool enable = false);


public:
	Direction Main(Mat& frame);


private:
	static RoadTracer* sInstance;

public:
	static RoadTracer* Instance();
	static void Release();
};