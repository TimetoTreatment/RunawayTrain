#include "console.h"


////////////////////
/* �ؽ�Ʈ �� ���� */
////////////////////
void Console::SetColor(Color foreground, Color background)
{
	int colorValue;

	if (foreground == DEFAULT) foreground = GRAY;	// �⺻ �� 7
	if (background == DEFAULT) background = BLACK;	// �⺻ �� 0
	colorValue = (foreground * 16) + background;	// ������ ��ȯ�� ���� �����ϰ� ����

	SetConsoleTextAttribute(mHandleConsole, colorValue);	// ������ ���� �Լ� �̿�, ���� ����
}


void Console::CursorVisible(bool visible)
{
	if (visible)
		mCursor.bVisible = TRUE;
	else
		mCursor.bVisible = FALSE;


	SetConsoleCursorInfo(mHandleConsole, &mCursor);
}


////////////////////
/* Ŀ�� ��ġ ���� */
////////////////////
void Console::CursorPosition(int x, int y)
{
	SetConsoleCursorPosition(mHandleConsole, { (short)x, (short)y });	// ������ ���� �Լ� �̿�, Ŀ�� ��ġ ����
}


/////////////////////////
/* �ܼ� â ��ü ���� */
/////////////////////////
void Console::Clear()
{
	mEraser.resize(130, ' ');					// ���찳 �� ����

	CursorPosition(0, 0);						// �ܼ��� ���� ��ܺ���

	for (int count = 0; count < 45; count++)	// �ܼ��� �ุŭ �ݺ�
		printf("%s\n", mEraser.c_str());		// ���� ���ڿ��� ���찳 ���

	CursorPosition(0, 0);
}


/////////////////////////
/* �ܼ� â �Ϻ� ���� */
/////////////////////////
void Console::Clear(int XPosStart, int YPosStart, int width, int height)
{
	mEraser.resize(width, ' ');							// ���찳 �� ����

	for (int count = 0; count < height; count++)		// ���̸�ŭ �ݺ�
	{
		CursorPosition(XPosStart, YPosStart + count);	// ������ ��ǥ���� �Ʒ���
		printf("%s\n", mEraser.c_str());				// ���� ���ڿ��� ���찳 ���
	}
}


//////////////////////////////
/* Draw TXT File to Console */
//////////////////////////////
void Console::Draw(std::string strOrFile, int startX, int startY, Color color)
{
	// ���ڰ� "*.txt"
	if (4 <= strOrFile.size() && strOrFile.substr(strOrFile.size() - 4) == ".txt")
	{
		std::string line;
		std::fstream file(strOrFile, std::ios_base::in);	// �ؽ�Ʈ ���� �б�

		// SetColor(color);

		for (; !file.eof(); startY++)	// ���� ����, ������ ������, y��ǥ ������Ʈ
		{
			std::getline(file, line);			// �ؽ�Ʈ ���� �� �� �б�

			CursorPosition(startX, startY);		// ��ǥ ����
			std::cout << line;					// ���
		}

		file.close();
	}
	// ���ڰ� "*.txt" �ƴ�
	else
	{
		size_t pos = 0;
		size_t prePos = 0;

		// SetColor(color);

		for (int count = 0; pos < strOrFile.size(); count++)				// �ุŭ �ݺ�
		{
			pos = strOrFile.find('\n', pos + 1);							// ���� ���� ��ġ ����
			CursorPosition(startX, startY + count);							// ��ǥ ������Ʈ
			std::cout << strOrFile.substr(prePos, pos - prePos) << '\n';	// ���ڿ��� �ڸ� �� ���
			prePos = pos + 1;
		}
	}
}


Console* Console::sInstance = nullptr;

Console* Console::Instance()
{
	if (sInstance == nullptr)
		sInstance = new Console;

	return sInstance;
}

void Console::Release()
{
	delete sInstance;
	sInstance = nullptr;
}
