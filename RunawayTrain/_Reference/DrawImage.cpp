#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main()
{
	Mat imageOriginal = imread("shaco.jpg", IMREAD_COLOR);
	Mat imageGrayscale;
	Mat imageBinary;

	namedWindow("Original Image", WINDOW_AUTOSIZE);
	namedWindow("Grayscale Image", WINDOW_AUTOSIZE);
	namedWindow("Binary Image", WINDOW_AUTOSIZE);

	cvtColor(imageOriginal, imageGrayscale, COLOR_RGB2GRAY);

	// threshold(imageGrayscale, imageBinary, 127, 255, THRESH_BINARY);

	imageBinary = imageGrayscale > 128;

	imshow("Original Image", imageOriginal);
	imshow("Grayscale Image", imageGrayscale);
	imshow("Binary Image", imageBinary);




	waitKey();

	return 0;
}