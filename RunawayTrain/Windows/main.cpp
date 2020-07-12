#define TEST 1


#if !TEST
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


#if TEST
#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include "LineTracer.h"
#include "WindowsCapture.h"

using namespace std;
using namespace cv;

int main(void)
{
	WindowsCapture windowsCapture(0.5);
	Mat original;
	Mat result;

	for (int frame = 0;; ++frame)
	{
		original = windowsCapture.Read();

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