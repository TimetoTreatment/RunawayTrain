#define TEMPLATEMATCHING
// TEMP
// LINETRACER
// WINDOWSCAPTURE
// LIGHT
// TEMPLATEMATCHING

#ifdef TEMP
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{

	// Read image
	Mat im = imread("blob.jpg", IMREAD_GRAYSCALE);

	// Setup SimpleBlobDetector parameters.
	SimpleBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 10;
	params.maxThreshold = 200;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 10;
	params.maxArea = 200;

	//// Filter by Circularity
	//params.filterByCircularity = true;
	//params.minCircularity = 0.9;

	//// Filter by Convexity
	//params.filterByConvexity = true;
	//params.minConvexity = 0.87;

	//// Filter by Inertia
	//params.filterByInertia = true;
	//params.minInertiaRatio = 0.01;


	// Storage for blobs
	vector<KeyPoint> keypoints;

	// Set up detector with params
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs
	detector->detect(im, keypoints);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob

	Mat im_with_keypoints;
	drawKeypoints(im, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show blobs
	imshow("keypoints", im_with_keypoints);
	waitKey(0);

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
	Light* light = new Light;

	light->Main();

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