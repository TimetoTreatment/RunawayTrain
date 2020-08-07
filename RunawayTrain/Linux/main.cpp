#define MOTOR
// TEMP
// LINETRACER
// WINDOWSCAPTURE
// LIGHT
// TEMPLATEMATCHING
// MOTOR
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

#ifdef MOTOR
#include "Motor.h"

int main(void)
{
	Motor motor;
	motor.Test();
}

#endif
