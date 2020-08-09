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

	float mXMarginTop = 0;	// 267
	float mXMarginBottom = 0;	// 66
	float mYPos = 240;			// 480
	float mYSize = 240;			// 66

	Point2f mSrcROI[4] = {
		{mXMarginTop,mYPos}, {640 - mXMarginTop,mYPos},
		{mXMarginBottom, mYPos + mYSize}, {640 - mXMarginBottom, mYPos + mYSize}
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

	Direction* mDirection;
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

	void Main(Mat& frameOriginal, Mat& frameFinal, Direction* direction);


private:
	static RoadTracer* sInstance;

public:
	static RoadTracer* Instance();
	static void Release();
};