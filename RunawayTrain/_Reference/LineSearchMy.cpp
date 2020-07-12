#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;



Mat region_of_interest(Mat img_edges, Point* points)
{
	/*
	Applies an image mask.

	Only keeps the region of the image defined by the polygon
	formed from `vertices`. The rest of the image is set to black.
	*/

	Mat img_mask = Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC1);


	Scalar ignore_mask_color = Scalar(255, 255, 255);
	const Point* ppt[1] = { points };
	int npt[] = { 4 };


	//filling pixels inside the polygon defined by "vertices" with the fill color
	fillPoly(img_mask, ppt, npt, 1, Scalar(255, 255, 255), LINE_8);


	//returning the image only where mask pixels are nonzero
	Mat img_masked;
	bitwise_and(img_edges, img_mask, img_masked);


	imshow("bd", img_mask);

	waitKey(1);


	return img_masked;
}




int main()
{
	Scalar red(0, 0, 255);
	Scalar blue(255, 0, 0);
	Scalar yellow(0, 255, 255);

	Scalar magenta(255, 0, 255);


	Mat rgb_color(1, 1, CV_8UC3, red);
	Mat hsv_color;

	cvtColor(rgb_color, hsv_color, COLOR_BGR2HSV);

	int range_count = 0;

	int hue = (int)hsv_color.at<Vec3b>(0, 0)[0];
	int saturation = (int)hsv_color.at<Vec3b>(0, 0)[1];
	int value = (int)hsv_color.at<Vec3b>(0, 0)[2];

	cout << "hue = " << hue << endl;
	cout << "saturation = " << saturation << endl;
	cout << "value = " << value << endl;

	int low_hue = hue - 10;
	int high_hue = hue + 10;

	int low_hue1 = 0, low_hue2 = 0;
	int high_hue1 = 0, high_hue2 = 0;

	if (low_hue < 10) {
		range_count = 2;

		high_hue1 = 180;
		low_hue1 = low_hue + 180;
		high_hue2 = high_hue;
		low_hue2 = 0;
	}
	else if (high_hue > 170) {
		range_count = 2;

		high_hue1 = low_hue;
		low_hue1 = 180;
		high_hue2 = high_hue - 180;
		low_hue2 = 0;
	}
	else {
		range_count = 1;

		low_hue1 = low_hue;
		high_hue1 = high_hue;
	}

	cout << low_hue1 << "  " << high_hue1 << endl;
	cout << low_hue2 << "  " << high_hue2 << endl;

	VideoCapture cap("roadtest2.mp4");
	Mat img_original, img_frame, img_hsv;


	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	Mat img_edge, img_gray;
	Mat img_canvus(Size(640, 360), CV_8UC3);
	vector<Vec4i> lines;
	Vec4i L;
	double t;

	float rho = 2; // distance resolution in pixels of the Hough grid
	float theta = 1 * CV_PI / 180; // angular resolution in radians of the Hough grid
	float hough_threshold = 15;	 // minimum number of votes(intersections in Hough grid cell)
	float minLineLength = 10; //minimum number of pixels making up a line
	float maxLineGap = 10;	//maximum gap in pixels between connectable line segments

	double trap_bottom_width = 1;  // width of bottom edge of trapezoid, expressed as percentage of image width
	double trap_top_width = 0.5;     // ditto for top edge of trapezoid
	double trap_height = 0.4;         // height of the trapezoid expressed as percentage of image height
	double trap_offset_height = 0.2;
	int width, height;

	for (int frame = 0;; frame++)
	{
		t = (double)getTickCount();

		// wait for a new frame from camera and store it into 'frame'
		cap.read(img_frame);

		// check if we succeeded
		if (img_frame.empty()) {
			cerr << "END of video\n";
			break;
		}

		resize(img_frame, img_frame, { 640,360 });

		//그레이 스케일 영상으로 변환.
		cvtColor(img_frame, img_gray, COLOR_BGR2GRAY);

		GaussianBlur(img_gray, img_gray, Size(7, 7), 0, 0);

		//캐니에지를 이용하여 에지 성분 검출
		Canny(img_gray, img_edge, 100, 200, 3, true);

		width = img_gray.cols;
		height = img_gray.rows;

		trap_bottom_width = 0.8;  // width of bottom edge of trapezoid, expressed as percentage of image width
		trap_top_width = 0.6;     // ditto for top edge of trapezoid
		trap_height = 0.4;         // height of the trapezoid expressed as percentage of image height
		trap_offset_height = 0.8;

		Point points[4];
		points[0] = Point((width * (1 - trap_bottom_width)) / 2, height * trap_offset_height);
		points[1] = Point((width * (1 - trap_top_width)) / 2, height - height * trap_height);
		points[2] = Point(width - (width * (1 - trap_top_width)) / 2, height - height * trap_height);
		points[3] = Point(width - (width * (1 - trap_bottom_width)) / 2, height * trap_offset_height);

		img_edge = region_of_interest(img_edge, points);

		//직선 성분을 검출합니다.  
		HoughLinesP(img_edge, lines, rho, theta, hough_threshold, minLineLength, maxLineGap);

		img_canvus = Scalar(0, 0, 0);
		imshow("canvus", img_canvus);

		if (waitKey(1) >= 0)
			break;

		//검출한 직선을 영상에 그려줍니다.  
		for (int i = 0; i < lines.size(); i++)
		{
			L = lines[i];
			line(img_frame, Point(L[0], L[1]), Point(L[2], L[3]), Scalar(0, 0, 255), 1, LINE_AA);
			line(img_canvus, Point(L[0], L[1]), Point(L[2], L[3]), Scalar(0, 0, 255), 1, LINE_AA);
		}

		//namedWindow("edge", WINDOW_AUTOSIZE);
		//resizeWindow("edge", { 640, 360 });

		imshow("gray", img_gray);
		imshow("edge", img_edge);
		imshow("canvus", img_canvus);
		imshow("result", img_frame);

		t = ((double)getTickCount() - t) / getTickFrequency();

		cout << setw(4) << frame << " : ";
		cout << fixed << setprecision(4) << t;
		cout << "   lineNum : " << lines.size() << endl;

		if (0.03 > t)
		{
			if (waitKey((0.03 - t) * 1000) >= 0)
				break;
		}

		else if (waitKey(1) >= 0)
			break;
	}


	return 0;
}