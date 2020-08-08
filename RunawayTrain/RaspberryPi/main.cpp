#define MOTORCONTROLLER
// TEMP
// LINETRACER
// WINDOWSCAPTURE
// LIGHT
// TEMPLATEMATCHING
// MOTORCONTROLLER
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
}home 

#endif

#ifdef MOTORCONTROLLER
#include "MotorController.h"

int main(void)
{
	MotorController::Instance()->Test();
}

#endif
