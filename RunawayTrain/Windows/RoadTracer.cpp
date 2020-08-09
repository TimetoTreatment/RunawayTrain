#include "RoadTracer.h"


void RoadTracer::Preprocess()
{
	warpPerspective(mFrameOriginal, mFramePerspective, getPerspectiveTransform(mSrcROI, mDstROI), Size(400, 225));

	line(mFrameFinal, mSrcROI[0], mSrcROI[1], Scalar(0, 0, 255), 2);
	line(mFrameFinal, mSrcROI[1], mSrcROI[3], Scalar(0, 0, 255), 2);
	line(mFrameFinal, mSrcROI[3], mSrcROI[2], Scalar(0, 0, 255), 2);
	line(mFrameFinal, mSrcROI[2], mSrcROI[0], Scalar(0, 0, 255), 2);
}


void RoadTracer::Threshold()
{
	cvtColor(mFramePerspective, mFramePerspective, COLOR_BGR2GRAY);
	threshold(mFramePerspective, mFramePerspective, 80, 255, THRESH_BINARY);
	Canny(mFramePerspective, mFrameEdge, 50, 100, 3, false);
	HoughLines(mFrameEdge, mLines, 1, 1 * CV_PI / 180, 75);
	cvtColor(mFrameEdge, mFrameEdge, COLOR_GRAY2BGR);

	float rho, theta;
	Point pt1, pt2;
	double cosTheta, sinTheta;
	double x0, y0;

	for (size_t i = 0; i < mLines.size(); i++)
	{
		rho = mLines[i][0];
		theta = mLines[i][1];
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

	cvtColor(mFramePerspective, mFrameEdgeCenter, COLOR_GRAY2BGR);
}


void RoadTracer::Histrogram()
{
	int width = mFramePerspective.size().width;
	int height = mFramePerspective.size().height;

	mFramePerspective(Rect(0, height - 100, width, 100)).copyTo(mFrameHistogram);

	mHistrogramLane.resize(width);

	for (int i = 0; i < width; i++)
		mHistrogramLane[i] = (int)(sum(mFrameHistogram(Rect(i, 0, 1, 100)))[0]);
}


void RoadTracer::LaneFinder()
{
	vector<int>::iterator LeftPtr;
	vector<int>::iterator RightPtr;

	for (LeftPtr = mHistrogramLane.begin() + 180; LeftPtr != mHistrogramLane.begin(); LeftPtr--)
	{
		if (*LeftPtr >= 90)
			break;
	}

	for (RightPtr = mHistrogramLane.begin() + 220; RightPtr != mHistrogramLane.end(); RightPtr++)
	{
		if (*RightPtr >= 90)
			break;
	}

	mLeftLanePos = distance(mHistrogramLane.begin(), LeftPtr);
	mRightLanePos = distance(mHistrogramLane.begin(), RightPtr);

	line(mFrameEdgeCenter, Point2f(mLeftLanePos, 0), Point2f(mLeftLanePos, 225), Scalar(0, 255, 0), 2);
	line(mFrameEdgeCenter, Point2f(mRightLanePos, 0), Point2f(mRightLanePos, 225), Scalar(0, 255, 0), 2);
}


void RoadTracer::LaneCenter()
{
	mLaneCenter = (mRightLanePos - mLeftLanePos) / 2 + mLeftLanePos;
	mFrameCenter = 200;

	line(mFrameEdgeCenter, Point2f(mLaneCenter, 180), Point2f(mLaneCenter, 225), Scalar(0, 0, 255), 3);
	line(mFrameEdgeCenter, Point2f(mFrameCenter, 0), Point2f(mFrameCenter, 225), Scalar(255, 0, 0), 3);
}


void RoadTracer::Show()
{
	if (!mDebugProcessShow)
		return;

	namedWindow("Edge", WINDOW_AUTOSIZE);
	imshow("Edge", mFrameEdge);

	namedWindow("Center", WINDOW_AUTOSIZE);
	imshow("Center", mFrameEdgeCenter);
}


void RoadTracer::Calculate()
{
	bool horizon = false;
	mExit = false;
	double rho;
	double theta;

	for (int i = 0; !mExit && i < mLines.size(); i++)
	{
		theta = mLines[i][1];

		if (1.18 <= theta && theta <= 1.96)
		{
			horizon = true;

			for (int j = 0; j < mLines.size(); j++)
			{
				rho = mLines[j][0];
				theta = mLines[j][1];

				// 0.78 -> 45  
				// 1.57 -> 90
				// 2.36 -> 135

				if (!(1.18 <= theta && theta <= 1.96))
				{
					if (0.39 <= theta && theta <= 1.18)
					{
						*mDirection = Direction::RightTurn;
					}
					else if (1.96 <= theta && theta <= 2.75)
					{
						*mDirection = Direction::LeftTurn;
					}

					mExit = true;
					break;
				}
			}
		}
	}

	if (!horizon)
	{
		int xPos = mLaneCenter - mFrameCenter;

		if (xPos < -20)
			*mDirection = Direction::LeftCorrection;

		else if (xPos > 20)
			*mDirection = Direction::RightCorrection;

		else
			*mDirection = Direction::Forward;
	}
}

void RoadTracer::DebugMode(bool debugProcessShow, bool debugImageTest)
{
	mDebugProcessShow = debugProcessShow;
	mDebugImageTest = debugImageTest;
}


void RoadTracer::Main(Mat& frameOriginal, Mat& frameFinal, Direction* direction)
{
	mDirection = direction;

	frameOriginal.copyTo(mFrameOriginal);
	mFrameFinal = frameFinal;

	Preprocess();
	Threshold();
	Histrogram();
	LaneFinder();
	LaneCenter();
	Calculate();
	Show();
}


RoadTracer* RoadTracer::sInstance = nullptr;

RoadTracer* RoadTracer::Instance()
{
	if (sInstance == nullptr)
		sInstance = new RoadTracer;

	return sInstance;
}

void RoadTracer::Release()
{
	delete sInstance;
	sInstance = nullptr;
}