#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;


class TemplateMatching
{
public:
	typedef struct _PointInfo
	{
		int x;
		int y;
		int sad;
	}PointInfo;


	void Main()
	{
		Mat img_color = imread("assets/image/Original.png", IMREAD_COLOR);
		Mat img_original;
		cvtColor(img_color, img_original, COLOR_BGR2GRAY);

		int subCols = img_color.cols / 50;
		int subRows = img_color.rows / 50;

		Mat img_template = imread("assets/image/Right/1.png", IMREAD_GRAYSCALE);

		for (int ab = 0; ab < 50; ab++)
		{
			imshow("half", img_template);

			cvtColor(img_color, img_original, COLOR_BGR2GRAY);

			imshow("ori", img_original);

			int original_h = img_original.rows;
			int original_w = img_original.cols;
			int template_h = img_template.rows;
			int template_w = img_template.cols;


			int best_position_y = 0;
			int best_position_x = 0;
			int best_position_sad = 200000;

			vector<PointInfo> point;


			for (int original_y = 0; original_y < original_h - template_h; original_y++)
			{
				uchar* data_original = img_original.data;

				for (int original_x = 0; original_x < original_w - template_w; original_x++)
				{
					int SAD = 0;

					for (int template_y = 0; template_y < template_h; template_y++)
					{
						uchar* data_template = img_template.data;

						for (int template_x = 0; template_x < template_w; template_x++)
						{

							int original_pixel = data_original[(original_y + template_y) * original_w + (original_x + template_x)];
							int template_pixel = data_template[template_y * template_w + template_x];

							SAD += abs(original_pixel - template_pixel);
						}
					}



					if (best_position_sad > SAD)
					{
						best_position_sad = SAD;
						best_position_y = original_y;
						best_position_x = original_x;
					}

					PointInfo pointInfo = { original_x, original_y, SAD };
					point.push_back(pointInfo);

				}
			}

			bool isFind = false;
			int min = 1000000;

			for (int i = 0; i < point.size(); i++)
			{
				if (min > abs(best_position_sad - point[i].sad))
					min = abs(best_position_sad - point[i].sad);


				if (abs(best_position_sad - point[i].sad) < 5000) // �Ӱ谪
				{
					cout << "Detect(->) : " << abs(best_position_sad - point[i].sad) << endl;
					rectangle(img_color, Point(point[i].x, point[i].y), Point(point[i].x + template_w, point[i].y + template_h), Scalar(255, 0, 0), 3);;
					isFind = true;
				}
			}

			if (isFind == false)
				cout << "Not Detected :" << min << '\n';

			imshow("template", img_template);
			imshow("Result", img_color);
			resize(img_color, img_color, { img_color.cols - subCols,img_color.rows - subRows });
			waitKey(1);
		}
	}

};