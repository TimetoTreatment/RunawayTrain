#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


class Light
{
public:

	void setLabel(Mat& image, string str, vector<Point> contour)
	{
		int fontface = FONT_HERSHEY_SIMPLEX;
		double scale = 0.5;
		int thickness = 1;
		int baseline = 0;

		Size text = getTextSize(str, fontface, scale, thickness, &baseline);
		Rect r = boundingRect(contour);

		Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
		rectangle(image, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(200, 200, 200), FILLED);
		putText(image, str, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
	}


	int Main()
	{
		int range_count = 0;

		Scalar red(100, 100, 150);

		Mat rgb_color = Mat(100, 100, CV_8UC3, red);
		Mat hsv_color;

		cvtColor(rgb_color, hsv_color, COLOR_BGR2HSV);

		imshow("asdf", rgb_color);

		int hue = (int)hsv_color.at<Vec3b>(0, 0)[0];
		int saturation = (int)hsv_color.at<Vec3b>(0, 0)[1];
		int value = (int)hsv_color.at<Vec3b>(0, 0)[2];


		cout << "hue = " << hue << endl;
		cout << "saturation = " << saturation << endl;
		cout << "value = " << value << endl;

		int low_hue = hue - 15;
		int high_hue = hue + 15;

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


		VideoCapture cap("stop.avi");
		Mat img_frame, img_hsv;


		if (!cap.isOpened()) {
			cerr << "ERROR! Unable to open camera\n";
			return -1;
		}


		for (;;)
		{
			// wait for a new frame from camera and store it into 'frame'
			cap.read(img_frame);

			//resize(img_frame, img_frame, Size(img_frame.cols * 0.3, img_frame.rows * 0.3));

			// check if we succeeded
			if (img_frame.empty()) {
				cerr << "ERROR! blank frame grabbed\n";
				break;
			}

			//HSV로 변환
			cvtColor(img_frame, img_hsv, COLOR_BGR2HSV);

			GaussianBlur(img_frame, img_frame, Size(7, 7), 0);

			//지정한 HSV 범위를 이용하여 영상을 이진화
			Mat img_mask1, img_mask2;
			inRange(img_hsv, Scalar(low_hue1, 50, 50), Scalar(high_hue1, 255, 255), img_mask1);
			if (range_count == 2) {
				inRange(img_hsv, Scalar(low_hue2, 50, 50), Scalar(high_hue2, 255, 255), img_mask2);
				img_mask1 |= img_mask2;
			}



			/////////////
			/* 원 검출 */
			/////////////
			/*

			//morphological closing 영역의 구멍 메우기
			dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			img_mask1 = ~img_mask1;

			// Setup SimpleBlobDetector parameters.
			SimpleBlobDetector::Params params;

			// Change thresholds
			params.minThreshold = 10;


			// Filter by Area.
			//params.filterByArea = true;
			//params.minArea = 5;

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
			detector->detect(img_mask1, keypoints);

			// Draw detected blobs as red circles.
			// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
			// the size of the circle corresponds to the size of blob

			Mat im_with_keypoints;

			drawKeypoints(img_mask1, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

			if (!keypoints.empty())
				cout << "red\n";
			else
				cout << "green\n";

			// Show blobs
			imshow("keypoints", im_with_keypoints);

			imshow("이진화 영상", img_mask1);
			imshow("원본 영상", img_frame);


			if (waitKey(10) >= 0)
				break;
			*/


			//morphological closing 영역의 구멍 메우기 
			dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(9, 9)));
			erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(9, 9)));



			vector<vector<Point> > contours;

			findContours(img_mask1, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

			for (int i = 0; i < contours.size(); i++)
				drawContours(img_mask1, contours, 0, (255, 0, 0), 3);

			imshow("contour", img_mask1);


			//라벨링 
			Mat img_labels, stats, centroids;
			int numOfLables = connectedComponentsWithStats(img_mask1, img_labels, stats, centroids, 8, CV_32S);

			//영역박스 그리기
			int max = -1, idx = 0;
			for (int j = 1; j < numOfLables; j++) {
				int area = stats.at<int>(j, CC_STAT_AREA);
				if (max < area)
				{
					max = area;
					idx = j;
				}
			}

			int left = stats.at<int>(idx, CC_STAT_LEFT);
			int top = stats.at<int>(idx, CC_STAT_TOP);
			int width = stats.at<int>(idx, CC_STAT_WIDTH);
			int height = stats.at<int>(idx, CC_STAT_HEIGHT);

			rectangle(img_frame, Point(left, top), Point(left + width, top + height),
				Scalar(0, 0, 255), 1);

			//if (width >= 200)
			//	cout << "go\n";
			//else if (width >= 30 || height >= 30)
			//	cout << "stop\n";

			if (max > 500)
			{
				cout << max << " : stop\n";

			}

			imshow("이진화 영상", img_mask1);
			imshow("원본 영상", img_frame);


			if (waitKey(1) >= 0)
				break;
		}


		return 0;
	}
};