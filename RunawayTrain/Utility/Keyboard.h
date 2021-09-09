//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Keyboard
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/*
	1. Features
		- Provide Non-Blocking keyboard input
		- Handle special keys easily

	2. How to use
		-
		-
*/

#pragma once
#include <Windows.h>


class Keyboard
{
public:

	bool UpPressed()
	{
		return GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W') || GetAsyncKeyState('w');
	}

	bool DownPressed()
	{
		return GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S') || GetAsyncKeyState('s');
	}

	bool LeftPressed()
	{
		return GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A') || GetAsyncKeyState('a');
	}

	bool RightPressed()
	{
		return GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D') || GetAsyncKeyState('d');
	}




	//int Block()
	//{
	//	int ch = _getch();
	//	if (ch == ARROW)
	//		ch = _getch();
	//	return ch;
	//}
	//int NonBlock()
	//{
	//	int ch = 0;
	//	if (_kbhit())
	//	{
	//		ch = _getch();
	//		if (ch == ARROW)
	//			ch = _getch();
	//	}
	//	return ch;
	//}
	//enum Key
	//{
	//	ENTER = 13,
	//	ARROW = 224,
	//	UP = 72,
	//	DOWN = 80,
	//	LEFT = 75,
	//	RIGHT = 77,
	//};
};

