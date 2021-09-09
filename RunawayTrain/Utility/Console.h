//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// 콘솔 출력 보조
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/*
	1. 개요
		- 콘솔 출력 보조
		- 코드의 길이를 줄일 수 있음

	2. 기능
		- 출력 커서 위치 지정
		- 출력 색상 지정
		- 콘솔 창 전체 또는 일부 비우기
		- 텍스트 파일 출력
		- 위 기능을 모두 합친 종합 출력
*/

#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>


class Console
{
private:

	HANDLE mHandleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO mCursor;
	std::string mEraser;

	/* 색상 문자열을 정수로 변환 */

	enum Color
	{
		DEFAULT = -1,
		RANDOM = 1,
		RED = 12,
		GREEN = 10,
		BLUE = 3,
		PURPLE = 13,
		YELLOW = 14,
		WHITE = 15,
		BLACK = 0,
		GRAY = 7,
	};


	Console() 
	{
		mCursor.dwSize = 100;
		mCursor.bVisible = FALSE;
	}
	~Console() {}


public:

	/* 커서 표시 */
	void CursorVisible(bool visible);

	/* 커서 위치 지정 */
	void CursorPosition(int x, int y);

	/* 텍스트 색 지정 */
	void SetColor(Color foreground = Color::DEFAULT, Color background = Color::DEFAULT);

	/* 콘솔 창 비우기 */
	void Clear();

	/* 콘솔 창 일부 비우기 */
	void Clear(int XPosStart, int YPosStart, int width, int height);

	/* 문자열 또는 파일 출력 */
	void Draw(std::string strOrFile, int startX, int startY, Color color = Color::DEFAULT);


private:
	static Console* sInstance;

public:
	static Console* Instance();
	static void Release();
};


