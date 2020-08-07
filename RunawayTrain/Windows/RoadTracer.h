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

	Mat mFrameOriginal, framePers, frameRendered, frameEdge, frameFinal, frameFinalDuplicate;
	Mat ROILane;
	int LeftLanePos, RightLanePos, frameCenter, laneCenter, Result;
	bool Exit;
	vector<Vec2f> lines;
	Vec4i L;

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
		warpPerspective(mFrameOriginal, framePers, getPerspectiveTransform(ROI, Destination), Size(400, 225));

		line(mFrameOriginal, ROI[0], ROI[1], Scalar(0, 0, 255), 2);
		line(mFrameOriginal, ROI[1], ROI[3], Scalar(0, 0, 255), 2);
		line(mFrameOriginal, ROI[3], ROI[2], Scalar(0, 0, 255), 2);
		line(mFrameOriginal, ROI[2], ROI[0], Scalar(0, 0, 255), 2);
	}

	void Threshold()
	{
		resize(framePers, framePers, { 400,225 });
		cvtColor(framePers, frameRendered, COLOR_BGR2GRAY);
		GaussianBlur(frameRendered, frameRendered, Size(7, 7), 0, 0);
		threshold(frameRendered, frameRendered, 127, 255, THRESH_BINARY);


		frameFinal = frameRendered;
		Canny(frameRendered, frameEdge, 50, 100, 3, false);

		HoughLines(frameEdge, lines, 1, 1 * CV_PI / 180, 100);
		cvtColor(frameEdge, frameEdge, COLOR_GRAY2BGR);

		float rho, theta;
		Point pt1, pt2;
		double a, b;
		double x0, y0;

		for (size_t i = 0; i < lines.size(); i++)
		{
			rho = lines[i][0], theta = lines[i][1];
			a = cos(theta);
			b = sin(theta);
			x0 = a * rho;
			y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(frameEdge, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
		}

		imshow("HoughLines", frameEdge);

		cvtColor(frameFinal, frameFinal, COLOR_GRAY2BGR);
		//cvtColor(frameFinal, frameFinalDuplicate, COLOR_RGB2BGR);   //used in histrogram function only
		frameFinal.copyTo(frameFinalDuplicate);
	}

	void Histrogram()
	{
		int width = frameRendered.size().width;

		histrogramLane.resize(400);
		histrogramLane.clear();

		for (int i = 0; i < width; i++)
		{
			ROILane = frameFinalDuplicate(Rect(i, 125, 1, 100));
			divide(255, ROILane, ROILane);
			histrogramLane.push_back((int)(sum(ROILane)[0]));
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

		line(frameFinal, Point2f(LeftLanePos, 0), Point2f(LeftLanePos, 225), Scalar(0, 255, 0), 2);
		line(frameFinal, Point2f(RightLanePos, 0), Point2f(RightLanePos, 225), Scalar(0, 255, 0), 2);
	}

	void LaneCenter()
	{
		laneCenter = (RightLanePos - LeftLanePos) / 2 + LeftLanePos;
		frameCenter = 199;

		line(frameFinal, Point2f(laneCenter, 180), Point2f(laneCenter, 225), Scalar(0, 0, 255), 3);
		line(frameFinal, Point2f(frameCenter, 0), Point2f(frameCenter, 225), Scalar(255, 0, 0), 3);

		Result = laneCenter - frameCenter;
	}


public:
	Direction Main(Mat& frame)
	{
		mDirection = Direction::Forward;

		string ResultStr;


		frame.copyTo(mFrameOriginal);

		resize(mFrameOriginal, mFrameOriginal, { 1280, 720 });

		Preprocess();

		mFrameOriginal = imread("assets/image/roadtestleft.png");
		framePers = mFrameOriginal;
		imshow("road", mFrameOriginal);

		Threshold();
		Histrogram();
		LaneFinder();
		LaneCenter();

		//if (Result < -10)
		//	ResultStr = "Left";
		//else if (Result > 10)
		//	ResultStr = "right";
		//else
		//	ResultStr = "Foward";

		//cout << "Direction = " << ResultStr << endl;

		putText(mFrameOriginal, ResultStr, Point2f(50, 100), 0, 2, Scalar(0, 0, 255), 2);

		namedWindow("orignal", WINDOW_NORMAL);
		imshow("orignal", mFrameOriginal);

		namedWindow("Perspective", WINDOW_NORMAL);
		imshow("Perspective", framePers);

		namedWindow("Final", WINDOW_NORMAL);
		imshow("Final", frameFinal);

		bool horizon = false;

		int quit = false;



		for (int i = 0; !quit && i < lines.size(); i++)
		{
			double theta = lines[i][1];

			if (1.3 <= theta && theta <= 1.9)
			{
				horizon = true;

				for (int j = 0; j < lines.size(); j++)
				{
					int rho = lines[j][0];
					theta = lines[j][1];


					// 0.78 = 우회전	45도  
					// 1.57 = 수직		90도
					// 2.36 = 좌회전	135도

					if (!(1.3 <= theta && theta <= 1.9))
					{
						if (0.6 <= theta && theta <= 1.0)
						{
							mDirection = Direction::Right90;
							cout << "Go Right\n";
						}
						else if (2.2 <= theta && theta <= 2.6)
						{
							mDirection = Direction::Left90;
							cout << "Go Left\n";
						}

						quit = true;
						break;
					}
				}
			}
		}

	}

private:
	static RoadTracer* sInstance;

public:
	static RoadTracer* Instance();
	static void Release();
};





/*
#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;
using namespace cv;
using namespace raspicam;

Mat mFrame, Matrix, framePers, frameGray, frameThresh, frameEdge, frameFinal, frameFinalDuplicate;
Mat ROILane;
int LeftLanePos, RightLanePos, frameCenter, laneCenter, Result;

RaspiCam_Cv Camera;

stringstream ss;


vector<int> histrogramLane;

Point2f Source[] = {Point2f(40,145),Point2f(360,145),Point2f(10,195), Point2f(390,195)};
Point2f Destination[] = {Point2f(100,0),Point2f(280,0),Point2f(100,240), Point2f(280,240)};


 void Setup ( int argc,char **argv, RaspiCam_Cv &Camera )
  {
	Camera.set ( CAP_PROP_FRAME_WIDTH,  ( "-w",argc,argv,400 ) );
	Camera.set ( CAP_PROP_FRAME_HEIGHT,  ( "-h",argc,argv,240 ) );
	Camera.set ( CAP_PROP_BRIGHTNESS, ( "-br",argc,argv,50 ) );
	Camera.set ( CAP_PROP_CONTRAST ,( "-co",argc,argv,50 ) );
	Camera.set ( CAP_PROP_SATURATION,  ( "-sa",argc,argv,50 ) );
	Camera.set ( CAP_PROP_GAIN,  ( "-g",argc,argv ,50 ) );
	Camera.set ( CAP_PROP_FPS,  ( "-fps",argc,argv,100));

}

void Capture()
{
	Camera.grab();
	Camera.retrieve( mFrame);
	cvtColor(mFrame, mFrame, COLOR_BGR2RGB);
}

void Perspective()
{
	line(mFrame,Source[0], Source[1], Scalar(0,0,255), 2);
	line(mFrame,Source[1], Source[3], Scalar(0,0,255), 2);
	line(mFrame,Source[3], Source[2], Scalar(0,0,255), 2);
	line(mFrame,Source[2], Source[0], Scalar(0,0,255), 2);


	Matrix = getPerspectiveTransform(Source, Destination);
	warpPerspective(mFrame, framePers, Matrix, Size(400,240));
}

void Threshold()
{
	cvtColor(framePers, frameGray, COLOR_RGB2GRAY);
	inRange(frameGray, 200, 255, frameThresh);
	Canny(frameGray,frameEdge, 900, 900, 3, false);
	add(frameThresh, frameEdge, frameFinal);
	cvtColor(frameFinal, frameFinal, COLOR_GRAY2RGB);
	cvtColor(frameFinal, frameFinalDuplicate, COLOR_RGB2BGR);   //used in histrogram function only

}

void Histrogram()
{
	histrogramLane.resize(400);
	histrogramLane.clear();

	for(int i=0; i<400; i++)       //mFrame.size().width = 400
	{
	ROILane = frameFinalDuplicate(Rect(i,140,1,100));
	divide(255, ROILane, ROILane);
	histrogramLane.push_back((int)(sum(ROILane)[0]));
	}
}

void LaneFinder()
{
	vector<int>:: iterator LeftPtr;
	LeftPtr = max_element(histrogramLane.begin(), histrogramLane.begin() + 150);
	LeftLanePos = distance(histrogramLane.begin(), LeftPtr);

	vector<int>:: iterator RightPtr;
	RightPtr = max_element(histrogramLane.begin() +250, histrogramLane.end());
	RightLanePos = distance(histrogramLane.begin(), RightPtr);

	line(frameFinal, Point2f(LeftLanePos, 0), Point2f(LeftLanePos, 240), Scalar(0, 255,0), 2);
	line(frameFinal, Point2f(RightLanePos, 0), Point2f(RightLanePos, 240), Scalar(0,255,0), 2);
}

void LaneCenter()
{
	laneCenter = (RightLanePos-LeftLanePos)/2 +LeftLanePos;
	frameCenter = 188;

	line(frameFinal, Point2f(laneCenter,0), Point2f(laneCenter,240), Scalar(0,255,0), 3);
	line(frameFinal, Point2f(frameCenter,0), Point2f(frameCenter,240), Scalar(255,0,0), 3);

	Result = laneCenter-frameCenter;
}


int main(int argc,char **argv)
{

	Setup(argc, argv, Camera);
	cout<<"Connecting to camera"<<endl;
	if (!Camera.open())
	{

	cout<<"Failed to Connect"<<endl;
	 }

	 cout<<"Camera Id = "<<Camera.getId()<<endl;




	while(1)
	{
	auto start = std::chrono::system_clock::now();

	Capture();
	Perspective();
	Threshold();
	Histrogram();
	LaneFinder();
	LaneCenter();

	ss.str(" ");
	ss.clear();
	ss<<"Result = "<<Result;
	putText(mFrame, ss.str(), Point2f(1,50), 0,1, Scalar(0,0,255), 2);


	namedWindow("orignal", WINDOW_KEEPRATIO);
	moveWindow("orignal", 0, 100);
	resizeWindow("orignal", 640, 480);
	imshow("orignal", mFrame);

	namedWindow("Perspective", WINDOW_KEEPRATIO);
	moveWindow("Perspective", 640, 100);
	resizeWindow("Perspective", 640, 480);
	imshow("Perspective", framePers);

	namedWindow("Final", WINDOW_KEEPRATIO);
	moveWindow("Final", 1280, 100);
	resizeWindow("Final", 640, 480);
	imshow("Final", frameFinal);


	waitKey(1);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;

	float t = elapsed_seconds.count();
	int FPS = 1/t;
	cout<<"FPS = "<<FPS<<endl;

	}


	return 0;

}
*/