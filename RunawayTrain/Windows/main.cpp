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
#define TEMPLATEMATCHING
// DRIVINGMANAGER
// TEMP
// ROADTRACER
// WINDOWSCAPTURE
// LIGHT
// TEMPLATEMATCHING
// MOTOR
// DETECTCIRCLE
// EXTRACTRECT


#ifdef TEMP
#include "opencv2/opencv.hpp"
#include "Temp.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Temp temp;
	temp.Main();
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


#ifdef WINDOWSCAPTURE
#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include "LineTracer.h"
#include "WindowsCapture.h"

using namespace std;
using namespace cv;

int main(void)
{
	WindowsCapture* windowsCapture = WindowsCapture::Instance(0.5);
	Mat original;
	Mat result;

	for (int frame = 0;; frame++)
	{
		original = windowsCapture->Read();

		result = original;
		//cvtColor(original, result, COLOR_BGR2GRAY);
		//Canny(result, result, 30, 90, 3);

		imshow("canny", result);

		waitKey(1);

		cout << frame << '\n';
	}

	return 0;
}

#endif


#ifdef LIGHT
#include "Light.h"

int main(void)
{
	Light light;

	light.Main();

	return 0;
}

#endif


#ifdef TEMPLATEMATCHING
#include "TemplateMatching.h"

int main(void)
{
	TemplateMatching* templateMatching = new TemplateMatching;

	templateMatching->Main();

	return 0;
}

#endif


#ifdef MOTOR
#include "Motor.h"

int main(void)
{
	Motor Motor.Main();

	return 0;
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