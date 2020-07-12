#pragma once
#include <Windows.h>
#include <iostream>
#include <opencv2/opencv.hpp>
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
	float mScale;
	HBITMAP mHbwindow;
	BITMAPINFOHEADER mBitmap;
	Mat mImage;

	WindowsCapture(float scale)
	{
		ocl::setUseOpenCL(true);

		mScale = scale;
		mHwnd = GetDesktopWindow();
		mHwindowDC = GetDC(mHwnd);
		mHwindowCompatibleDC = CreateCompatibleDC(mHwindowDC);
		SetStretchBltMode(mHwindowCompatibleDC, COLORONCOLOR);

		RECT windowsize;
		GetClientRect(mHwnd, &windowsize);

		mSourceHeight = windowsize.bottom;
		mSourceWidth = windowsize.right;
		mHeight = (int)(windowsize.bottom * mScale);
		mWidth = (int)(windowsize.right * mScale);

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

public:

	Mat& Read()
	{
		StretchBlt(mHwindowCompatibleDC, 0, 0, mWidth, mHeight, mHwindowDC, 0, 0, mSourceWidth, mSourceHeight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
		GetDIBits(mHwindowCompatibleDC, mHbwindow, 0, mHeight, mImage.data, (BITMAPINFO*)&mBitmap, DIB_RGB_COLORS);  //copy from mHwindowCompatibleDC to mHbwindow

		return mImage;
	};


private:
	static WindowsCapture* sInstance;

public:
	static WindowsCapture* Instance(float scale = 1)
	{
		if (sInstance == nullptr)
			sInstance = new WindowsCapture(scale);

		return sInstance;
	}

	void Release()
	{
		delete sInstance;
		sInstance = nullptr;
	}
};
