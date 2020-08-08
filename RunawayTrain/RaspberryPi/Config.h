#pragma once
#include <string>

using namespace std;

enum class MotorStatus
{
	LeftForward,
	LeftReverse,
	LeftStop,
	RightForward,
	RightReverse,
	RightStop,
};

enum class Direction
{
	Forward,
	Stop,
	LeftCorrection,
	RightCorrection,
	LeftTurn,
	RightTurn,
};


string DirectionStr(Direction direction);