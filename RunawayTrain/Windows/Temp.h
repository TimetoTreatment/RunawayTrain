#pragma once


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
	


	void Capture(VideoCapture& cap)
	{
		cap.read(frame);

		if (frame.empty()) {
			cerr << "END of video\n";
			exit(0);
		}
	}

	void Perspective()
	{
		Matrix = getPerspectiveTransform(Source, Destination);
		warpPerspective(frame, framePers, Matrix, Size(400, 240));

		line(frame, Source[0], Source[1], Scalar(0, 0, 255), 2);
		line(frame, Source[1], Source[3], Scalar(0, 0, 255), 2);
		line(frame, Source[3], Source[2], Scalar(0, 0, 255), 2);
		line(frame, Source[2], Source[0], Scalar(0, 0, 255), 2);
	}

	void Threshold()
	{
		cvtColor(framePers, frameGray, COLOR_RGB2GRAY);
		GaussianBlur(frameGray, frameGray, Size(13, 13), 0, 0);
		Canny(frameGray, frameEdge, 50, 150, 3, false);

		frameFinal = frameEdge;

		cvtColor(frameFinal, frameFinal, COLOR_GRAY2RGB);
		cvtColor(frameFinal, frameFinalDuplicate, COLOR_RGB2BGR);   //used in histrogram function only


		imshow("frameGray", frameGray);
		imshow("frameEdge", frameEdge);
	}

	void Histrogram()
	{
		histrogramLane.resize(400);
		histrogramLane.clear();

		for (int i = 0; i < 400; i++)       //frame.size().width = 400
		{
			ROILane = frameFinalDuplicate(Rect(i, 140, 1, 100));
			divide(255, ROILane, ROILane);
			histrogramLane.push_back((int)(sum(ROILane)[0]));
		}
	}

	void LaneFinder()
	{
		vector<int>::iterator LeftPtr;
		LeftPtr = max_element(histrogramLane.begin(), histrogramLane.begin() + 190);
		LeftLanePos = distance(histrogramLane.begin(), LeftPtr);

		vector<int>::iterator RightPtr;
		RightPtr = max_element(histrogramLane.begin() + 210, histrogramLane.end());

		if (*RightPtr == 0)
			RightPtr = histrogramLane.end() - 1;

		RightLanePos = distance(histrogramLane.begin(), RightPtr);

		line(frameFinal, Point2f(LeftLanePos, 0), Point2f(LeftLanePos, 240), Scalar(0, 255, 0), 2);
		line(frameFinal, Point2f(RightLanePos, 0), Point2f(RightLanePos, 240), Scalar(0, 255, 0), 2);
	}

	void LaneCenter()
	{
		laneCenter = (RightLanePos - LeftLanePos) / 2 + LeftLanePos;
		frameCenter = 188;

		line(frameFinal, Point2f(laneCenter, 180), Point2f(laneCenter, 240), Scalar(0, 0, 255), 3);
		line(frameFinal, Point2f(frameCenter, 0), Point2f(frameCenter, 240), Scalar(255, 0, 0), 3);

		Result = laneCenter - frameCenter;
	}


	void Main()
	{
		VideoCapture cap("assets/video/roadtest3.mp4");
		string ResultStr;

		while (1)
		{
			auto start = std::chrono::system_clock::now();

			Capture(cap);
			Perspective();
			Threshold();
			Histrogram();
			LaneFinder();
			LaneCenter();

			ss.str(" ");
			ss.clear();

			if (Result < -10)
				ResultStr = "Left";
			else if (Result > 10)
				ResultStr = "right";
			else
				ResultStr = "Foward";

			ss << "Direction = " << ResultStr;

			putText(frame, ss.str(), Point2f(50, 100), 0, 2, Scalar(0, 0, 255), 2);

			namedWindow("orignal", WINDOW_KEEPRATIO);
			moveWindow("orignal", 0, 100);
			resizeWindow("orignal", 640, 360);
			imshow("orignal", frame);

			namedWindow("Perspective", WINDOW_KEEPRATIO);
			moveWindow("Perspective", 640, 100);
			resizeWindow("Perspective", 640, 360);
			imshow("Perspective", framePers);

			namedWindow("Final", WINDOW_KEEPRATIO);
			moveWindow("Final", 1280, 100);
			resizeWindow("Final", 640, 360);
			imshow("Final", frameFinal);


			waitKey(1);
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end - start;

			float t = elapsed_seconds.count();
			int FPS = 1 / t;
			cout << "FPS = " << FPS << '\n' << ResultStr << endl;

		}
	}
};


