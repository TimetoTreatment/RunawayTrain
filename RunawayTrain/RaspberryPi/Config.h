#pragma once
#define RASPBERRYPI
#include <string>

using namespace std;

enum class MotorStatus
{
	Forward,
	Reverse,
	Stop,
	Left,
	Right,
	Center
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