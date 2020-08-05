#define LINETRACER
// TEMP
// LINETRACER
// WINDOWSCAPTURE
// LIGHT
// TEMPLATEMATCHING
// MOTOR
// DETECTCIRCLE


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


#ifdef LINETRACER
#include <opencv2/opencv.hpp>
#include "LineTracer.h"

using namespace cv;

int main(void)
{
	LineTracer lineTracer;
	lineTracer.Main();

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