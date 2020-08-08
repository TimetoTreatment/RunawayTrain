#define TEST 0

#if !TEST
#include "DrivingManager.h"
int main(void)
{
	DrivingManager::Instance()->MainLoop();
	DrivingManager::Release();

	return 0;
}
#endif


#if TEST
#define MOTORCONTROLLER
// DRIVINGMANAGER
// TEMP
// ROADTRACER
// WINDOWSCAPTURE
// LIGHT
// TEMPLATEMATCHING
// MOTORCONTROLLER
// DETECTCIRCLE
// EXTRACTRECT

#ifdef TEMP
#include "opencv2/opencv.hpp"
#include "Temp.h"

using namespace cv;
using namespace std;

int main(void)
{
	Mat image = imread("/home/pi/projects/RaspberryPi/blob.jpg");

	imshow("image", image);
}

#endif



#ifdef DETECTCIRCLE
#include "opencv2/opencv.hpp"
#include "DetectCircle.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	DetectCircle dc;
	dc.Main();
}

#endif


#ifdef ROADTRACER
#include <opencv2/opencv.hpp>
#include "RoadTracer.h"

using namespace cv;

int main(void)
{
	RoadTracer roadTracer;
	roadTracer.Main();

	return 0;
}
#endif


#ifdef MOTORCONTROLLER
#include "MotorController.h"

int main(void)
{
	MotorController::Instance()->Test();
}

#endif

#ifdef EXTRACTRECT
#include "ExtractRect.h"

int main(void)
{
	ExtractRect extractRect;
	extractRect.Main();

	return 0;
}

#endif

#endif