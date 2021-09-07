#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include "../Utility/TCP.h"

using namespace std;
using namespace cv;


int main()
{
	TCP* tcp = new TCP("9510", "192.168.219.102");

	Mat img = imread("1.png");
	int size = img.total() * img.channels();
	string str;

	for (;;)
	{
		switch (tcp->WaitEvent())
		{
		case TCP::WaitEventType::MESSAGE:
			str = tcp->ReadMessage();

			if (str == "ACCEPT")
			{
				tcp->Send("SIZE");

			}
			if (str == "SIZEREADY")
			{
				tcp->Send("6220800");
			}

			else if (str == "SIZEGOOD")
			{

				tcp->Send("BUFFER");
				char* buffer = new char[size+4096];
				memcpy(buffer, img.data, size);
				for (int i=0;i<size;)
				{
					tcp->Send(buffer + i);
					i += 4096;

				}
			}




			break;




		}



	}






	return 0;
}