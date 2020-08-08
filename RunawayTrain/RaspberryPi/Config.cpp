#include "Config.h"

string DirectionStr(Direction direction)
{
	switch (direction)
	{
	case Direction::Forward:			return "Forward";
	case Direction::Stop:				return "Stop";
	case Direction::LeftCorrection:		return "LeftCorrection";
	case Direction::RightCorrection:	return "RightCorrection";
	case Direction::LeftTurn:			return "LeftTurn";
	case Direction::RightTurn:			return "RightTurn";
	}
}