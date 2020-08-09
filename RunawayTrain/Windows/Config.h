#pragma once
#include <string>


enum class MotorStatus
{
	LeftForward,
	LeftReverse,
	LeftStop,
	RightForward,
	RightReverse,
	RightStop
};


enum class Direction
{
	Forward,
	Stop,
	LeftCorrection,
	RightCorrection,
	LeftTurn,
	RightTurn
};

std::string DirectionStr(Direction direction);


enum class Machine
{
	Windows,
	RaspberryPi
};