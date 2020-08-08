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

	bool mImageTest;
	bool mVideoShow;

	Mat mFrameOriginal, mFrameFinal;
	Mat mFramePerspective, mFrameEdge, mFrameEdgeCenter, mFrameHistogram;

	float mXMarginTop = 267;
	float mXMarginBottom = 66;
	float mYPos = 480;
	float mYSize = 66;

	Point2f mSrcROI[4] = {
		{mXMarginTop,mYPos}, {1280 - mXMarginTop,mYPos},
		{mXMarginBottom,mYPos + mYSize}, {1280 - mXMarginBottom,mYPos + mYSize}
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

	void Test(bool imgEnable = false);

	void Threshold();

	void Histrogram();

	void LaneFinder();

	void LaneCenter();

	void Show();

	void Calculate();


public:

	void DebugMode(bool videoShow = true, bool imageTest = false);

	Direction Main(Mat& frameOriginal, Mat& frameFinal);


private:
	static RoadTracer* sInstance;

public:
	static RoadTracer* Instance();
	static void Release();
};