//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// �ܼ� ��� ����
//
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/*
	1. ����
		- �ܼ� ��� ����
		- �ڵ��� ���̸� ���� �� ����

	2. ���
		- ��� Ŀ�� ��ġ ����
		- ��� ���� ����
		- �ܼ� â ��ü �Ǵ� �Ϻ� ����
		- �ؽ�Ʈ ���� ���
		- �� ����� ��� ��ģ ���� ���
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

	/* ���� ���ڿ��� ������ ��ȯ */

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

	/* Ŀ�� ǥ�� */
	void CursorVisible(bool visible);

	/* Ŀ�� ��ġ ���� */
	void CursorPosition(int x, int y);

	/* �ؽ�Ʈ �� ���� */
	void SetColor(Color foreground = Color::DEFAULT, Color background = Color::DEFAULT);

	/* �ܼ� â ���� */
	void Clear();

	/* �ܼ� â �Ϻ� ���� */
	void Clear(int XPosStart, int YPosStart, int width, int height);

	/* ���ڿ� �Ǵ� ���� ��� */
	void Draw(std::string strOrFile, int startX, int startY, Color color = Color::DEFAULT);


private:
	static Console* sInstance;

public:
	static Console* Instance();
	static void Release();
};


