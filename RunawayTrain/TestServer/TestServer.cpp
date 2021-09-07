#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include "../Utility/TCP.h"

using namespace std;
using namespace cv;
using namespace std::chrono_literals;

int main()
{
	TCP* tcp = new TCP("9510");

	int size = 0;
	string str;
	for (;;)
	{
		switch (tcp->WaitEvent(0))
		{
		case TCP::WaitEventType::NEWCLIENT:
			tcp->AddClient();
			tcp->Send("ACCEPT");
			break;

		case TCP::WaitEventType::MESSAGE:

			str = tcp->ReadMessage();


			if (str == "SIZE")
			{
				tcp->Send("SIZEREADY");
				tcp->WaitEvent();

				size = atoi(tcp->ReadMessage());
				tcp->Send("SIZEGOOD");

			}
			
			
			else if (str == "BUFFER")
			{
				char* buffer = new char[size+4096];



				for (int i = 0; i < size;)
				{
					tcp->WaitEvent();
					memcpy(buffer + i, tcp->ReadMessage(), 4096);
					i += 4096;

				}

				Mat image(Size(1920, 1080), CV_8UC3, buffer, Mat::AUTO_STEP);



				imshow("mat", image);

				waitKey(0);


			}


			break;

		}




	}










	return 0;
}