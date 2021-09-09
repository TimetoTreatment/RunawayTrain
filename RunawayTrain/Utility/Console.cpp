#include "console.h"


////////////////////
/* 텍스트 색 지정 */
////////////////////
void Console::SetColor(Color foreground, Color background)
{
	int colorValue;

	if (foreground == DEFAULT) foreground = GRAY;	// 기본 값 7
	if (background == DEFAULT) background = BLACK;	// 기본 값 0
	colorValue = (foreground * 16) + background;	// 정수로 변환한 값을 적절하게 대입

	SetConsoleTextAttribute(mHandleConsole, colorValue);	// 윈도우 제공 함수 이용, 색상 변경
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
/* 커서 위치 지정 */
////////////////////
void Console::CursorPosition(int x, int y)
{
	SetConsoleCursorPosition(mHandleConsole, { (short)x, (short)y });	// 윈도우 제공 함수 이용, 커서 위치 변경
}


/////////////////////////
/* 콘솔 창 전체 비우기 */
/////////////////////////
void Console::Clear()
{
	mEraser.resize(130, ' ');					// 지우개 폭 설정

	CursorPosition(0, 0);						// 콘솔의 좌측 상단부터

	for (int count = 0; count < 45; count++)	// 콘솔의 행만큼 반복
		printf("%s\n", mEraser.c_str());		// 공백 문자열인 지우개 출력

	CursorPosition(0, 0);
}


/////////////////////////
/* 콘솔 창 일부 비우기 */
/////////////////////////
void Console::Clear(int XPosStart, int YPosStart, int width, int height)
{
	mEraser.resize(width, ' ');							// 지우개 폭 설정

	for (int count = 0; count < height; count++)		// 높이만큼 반복
	{
		CursorPosition(XPosStart, YPosStart + count);	// 설정한 좌표부터 아래로
		printf("%s\n", mEraser.c_str());				// 공백 문자열인 지우개 출력
	}
}


//////////////////////////////
/* Draw TXT File to Console */
//////////////////////////////
void Console::Draw(std::string strOrFile, int startX, int startY, Color color)
{
	// 인자가 "*.txt"
	if (4 <= strOrFile.size() && strOrFile.substr(strOrFile.size() - 4) == ".txt")
	{
		std::string line;
		std::fstream file(strOrFile, std::ios_base::in);	// 텍스트 파일 읽기

		// SetColor(color);

		for (; !file.eof(); startY++)	// 색상 변경, 파일의 끝까지, y좌표 업데이트
		{
			std::getline(file, line);			// 텍스트 파일 한 줄 읽기

			CursorPosition(startX, startY);		// 좌표 설정
			std::cout << line;					// 출력
		}

		file.close();
	}
	// 인자가 "*.txt" 아님
	else
	{
		size_t pos = 0;
		size_t prePos = 0;

		// SetColor(color);

		for (int count = 0; pos < strOrFile.size(); count++)				// 행만큼 반복
		{
			pos = strOrFile.find('\n', pos + 1);							// 개행 문자 위치 저장
			CursorPosition(startX, startY + count);							// 좌표 업데이트
			std::cout << strOrFile.substr(prePos, pos - prePos) << '\n';	// 문자열을 자른 후 출력
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
