#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <windows.h>
#pragma comment(lib, "Winmm.lib")

using namespace std;
using namespace cv;


class LineTracer
{
public:
	Mat frame, Matrix, framePers, frameRendered, frameEdge, frameFinal, frameFinalDuplicate;
	Mat ROILane;
	int LeftLanePos, RightLanePos, frameCenter, laneCenter, Result;
	bool Exit;
	vector<Vec4i> lines;
	vector<Vec2f> liness;
	Vec4i L;

	stringstream ss;

	vector<int> histrogramLane;

	float xpos1 = 400;
	float xpos2 = 1520;
	float ypos1 = 720;
	float ypos2 = 820;
	int fps = 0;

	Point2f Source[4] = { {xpos1,ypos1}, {xpos2,ypos1}, {xpos1 - 300,ypos2}, {xpos2 + 300,ypos2} };
	Point2f Destination[4] = { Point2f(0,0) ,Point2f(400,0) ,Point2f(0,240) , Point2f(400,240) };

	void Capture(VideoCapture& cap)
	{
		cap.read(frame);
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
		cvtColor(framePers, frameRendered, COLOR_RGB2GRAY);

		imshow("frameGray", frameRendered);

		GaussianBlur(frameRendered, frameRendered, Size(7, 7), 0, 0);

		threshold(frameRendered, frameRendered, 127, 255, THRESH_BINARY);

		imshow("frameBinary", frameRendered);

		frameFinal = frameRendered;

		Canny(frameRendered, frameEdge, 50, 100, 3, false);

		float rho = 1; // distance resolution in pixels of the Hough grid
		float theta = 1 * CV_PI / 180; // angular resolution in radians of the Hough grid
		float hough_threshold = 15;	 // minimum number of votes(intersections in Hough grid cell)
		float minLineLength = 30; //minimum number of pixels making up a line
		float maxLineGap = 10;	//maximum gap in pixels between connectable line segments

		//HoughLinesP(frameEdge, lines, rho, theta, hough_threshold, minLineLength, maxLineGap);
		//cvtColor(frameEdge, frameEdge, COLOR_GRAY2BGR);

		//for (int i = 0; i < lines.size(); i++)
		//{
		//	L = lines[i];
		//	line(frameEdge, Point(L[0], L[1]), Point(L[2], L[3]), Scalar(0, 0, 255), 1, LINE_AA);
		//	line(frameEdge, Point(L[0], L[1]), Point(L[2], L[3]), Scalar(0, 0, 255), 1, LINE_AA);
		//}

		HoughLines(frameEdge, liness, 5, 5 * CV_PI / 180, 100, 0, 0);
		cvtColor(frameEdge, frameEdge, COLOR_GRAY2BGR);
		imshow("frameEdge", frameEdge);

		for (size_t i = 0; i < liness.size(); i++)
		{
			float rho = liness[i][0], theta = liness[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(frameEdge, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
		}

		imshow("HoughLines", frameEdge);

		cvtColor(frameFinal, frameFinal, COLOR_GRAY2RGB);
		cvtColor(frameFinal, frameFinalDuplicate, COLOR_RGB2BGR);   //used in histrogram function only
	}

	void Histrogram()
	{
		int width = frameRendered.size().width;

		histrogramLane.resize(400);
		histrogramLane.clear();

		for (int i = 0; i < width; i++)
		{
			ROILane = frameFinalDuplicate(Rect(i, 140, 1, 100));
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

		line(frameFinal, Point2f(LeftLanePos, 0), Point2f(LeftLanePos, 240), Scalar(0, 255, 0), 2);
		line(frameFinal, Point2f(RightLanePos, 0), Point2f(RightLanePos, 240), Scalar(0, 255, 0), 2);
	}

	void LaneCenter()
	{
		laneCenter = (RightLanePos - LeftLanePos) / 2 + LeftLanePos;
		frameCenter = 199;

		line(frameFinal, Point2f(laneCenter, 180), Point2f(laneCenter, 240), Scalar(0, 0, 255), 3);
		line(frameFinal, Point2f(frameCenter, 0), Point2f(frameCenter, 240), Scalar(255, 0, 0), 3);

		Result = laneCenter - frameCenter;
	}


	int Main()
	{
		VideoCapture cap("assets/video/roadtest3.mp4");
		string ResultStr;
		PlaySound(TEXT("assets/audio/laugh0.wav"), NULL, SND_ASYNC);

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
			cout << "FPS = " << FPS << '\n' << ResultStr << '\n' << "lineNum : " << liness.size() << endl;
		}

		return 0;

	}
};
