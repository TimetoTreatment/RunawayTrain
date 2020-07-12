#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main()
{
    Mat img_gray;

    img_gray = imread("input.jpg", IMREAD_GRAYSCALE);
    medianBlur(img_gray, img_gray, 5);
    Mat img_color;
    cvtColor(img_gray, img_color, COLOR_GRAY2BGR);


    vector<Vec3f> circles;
    HoughCircles(img_gray, circles, HOUGH_GRADIENT, 1, 20, 50, 35, 0, 0);


    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        Point center(c[0], c[1]);
        int radius = c[2];

        circle(img_color, center, radius, Scalar(0, 255, 0), 2);
        circle(img_color, center, 2, Scalar(0, 0, 255), 3);
    }

    imshow("detected circles", img_color);
    waitKey(0);
    destroyAllWindows();

    return 0;

}