#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Config.h"

using namespace std;
using namespace cv;


class RoadTracer
{
private:

	bool mDebugProcessShow;
	bool mDebugImageTest;

	Mat mFrameOriginal, mFrameFinal;
	Mat mFramePerspective, mFrameEdge, mFrameEdgeCenter, mFrameHistogram;

	float mXMarginTop = 100;	// 267
	float mXMarginBottom = 0;	// 66
	float mYPos = 360;			// 480
	float mYSize = 360;			// 66

	Point2f mSrcROI[4] = {
		{mXMarginTop,mYPos}, {1280 - mXMarginTop,mYPos},
		{mXMarginBottom, mYPos + mYSize}, {1280 - mXMarginBottom, mYPos + mYSize}
	};

	Point2f mDstROI[4] = {
		Point2f(0,0) ,
		Point2f(400,0) ,
		Point2f(0,225) ,
		Point2f(400,225)
	};

	vector<Vec2f> mLines;
	vector<int> mHistrogramLane;
	int mLeftLanePos, mRightLanePos, mFrameCenter, mLaneCenter;

	Direction mDirection;
	bool mExit;

	void Preprocess();

	void Threshold();

	void Histrogram();

	void LaneFinder();

	void LaneCenter();

	void Show();

	void Calculate();


public:

	void DebugMode(bool debugProcessShow = true, bool debugImageTest = false);

	Direction Main(Mat& frameOriginal, Mat& frameFinal);


private:
	static RoadTracer* sInstance;

public:
	static RoadTracer* Instance();
	static void Release();
};