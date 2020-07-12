#pragma once
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <iostream>
#include <opencv2/core/ocl.hpp>

using namespace std;
using namespace cv;

class WindowsCapture
{
private:

	HWND mHwnd;
	HDC mHwindowDC;
	HDC mHwindowCompatibleDC;
	int mHeight;
	int mWidth;
	int mSourceHeight;
	int mSourceWidth;
	HBITMAP mHbwindow;
	BITMAPINFOHEADER mBitmap;
	Mat mImage;

public:

	WindowsCapture(float scale = 1)
	{
		ocl::setUseOpenCL(true);

		mHwnd = GetDesktopWindow();
		mHwindowDC = GetDC(mHwnd);
		mHwindowCompatibleDC = CreateCompatibleDC(mHwindowDC);
		SetStretchBltMode(mHwindowCompatibleDC, COLORONCOLOR);

		RECT windowsize;
		GetClientRect(mHwnd, &windowsize);

		mSourceHeight = windowsize.bottom;
		mSourceWidth = windowsize.right;
		mHeight = (int)(windowsize.bottom * scale);
		mWidth = (int)(windowsize.right * scale);

		mImage.create(mHeight, mWidth, CV_8UC4);

		mHbwindow = CreateCompatibleBitmap(mHwindowDC, mWidth, mHeight);
		mBitmap.biSize = sizeof(BITMAPINFOHEADER);
		mBitmap.biWidth = mWidth;
		mBitmap.biHeight = -mHeight;
		mBitmap.biPlanes = 1;
		mBitmap.biBitCount = 32;
		mBitmap.biCompression = BI_RGB;
		mBitmap.biSizeImage = 0;
		mBitmap.biXPelsPerMeter = 0;
		mBitmap.biYPelsPerMeter = 0;
		mBitmap.biClrUsed = 0;
		mBitmap.biClrImportant = 0;

		SelectObject(mHwindowCompatibleDC, mHbwindow);
		Read();
	};

	~WindowsCapture()
	{
		DeleteObject(mHbwindow);
		DeleteDC(mHwindowCompatibleDC);
		ReleaseDC(mHwnd, mHwindowDC);
	};

	Mat& Read()
	{
		// copy from the window device context to the bitmap device context
		StretchBlt(mHwindowCompatibleDC, 0, 0, mWidth, mHeight, mHwindowDC, 0, 0, mSourceWidth, mSourceHeight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
		GetDIBits(mHwindowCompatibleDC, mHbwindow, 0, mHeight, mImage.data, (BITMAPINFO*)&mBitmap, DIB_RGB_COLORS);  //copy from mHwindowCompatibleDC to mHbwindow

		return mImage;
	};

	//void Main()
	//{
	//	ocl::setUseOpenCL(true);

	//	for (int count = 0;; count++)
	//	{
	//		Read();

	//		namedWindow("Desktop Capture", WINDOW_NORMAL);
	//		resizeWindow("Desktop Capture", 1440, 810);
	//		imshow("Desktop Capture", mImage);

	//		if (waitKey(1) >= 0)
	//			break;

	//		cout << count << "\n";
	//	}
	//}



};
