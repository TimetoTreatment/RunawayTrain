#include <iostream>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>

#include "MotorManager.h"

using namespace std;


int main()
{
	cv::VideoCapture videoCapture(0);

	
	MotorManager* mm = new MotorManager();

	char ch;

	mm->SpeedSteer(100);
	mm->SpeedDrive(100);

	for (;;)
	{
		switch()







	}

	delete mm;


	return 0;
}