#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "../Utility/TCPLinux.h"

using namespace std;
using namespace cv;


int main()
{

	VideoCapture cap(0);

	cap.set(CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CAP_PROP_FRAME_HEIGHT, 480);

	Mat image;

	cap >> image;

	int width = image.cols;
	int height = image.rows;
	int bpp = image.channels();
	int imagesize = width * height * bpp;

	std::vector<uchar> array;
	if (image.isContinuous()) {
		// array.assign(image.datastart, image.dataend); // <- has problems for sub-imagerix like image = big_image.row(i)
		array.assign(image.data, image.data + image.total() * image.channels());
	}
	else {
		for (int i = 0; i < image.rows; ++i) {
			array.insert(array.end(), image.ptr<uchar>(i), image.ptr<uchar>(i) + image.cols * image.channels());
		}
	}

	uchar* buffer = new uchar[imagesize+1];
	memcpy(buffer, image.data, imagesize);
	buffer[imagesize] = 0;



	return 0;
}