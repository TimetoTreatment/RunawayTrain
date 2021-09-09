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

	vector<uchar> imageEncoded;
	int size = 0;

	for (bool exit = false; !exit;)
	{
		switch (tcp->WaitEvent(0))
		{
		case TCP::WaitEventType::NEWCLIENT:
			tcp->AddClient();

			break;

		case TCP::WaitEventType::MESSAGE:

			if (tcp->ReadMessage() == "START")
			{
				tcp->Send("READY", 6);

				string str = tcp->ReadMessage();

				size = 0;

				for (const char* iter = str.c_str(); *iter; iter++)
				{
					if ('0' <= *iter && *iter <= '9')
					{
						size *= 10;
						size += *iter - '0';
					}
					else
						exit = true;
				}

				if (exit == true)
					break;

				const char* data = tcp->ReadData(size);

				imageEncoded.assign(data, data + size);

				Mat image = imdecode(imageEncoded, ImreadModes::IMREAD_COLOR);

				imshow("mat", image);
			}

			break;

		case TCP::WaitEventType::DISCONNECT:
			tcp->CloseClient();
			break;

		}

		if (waitKey(1) == 'q')
			break;

		cout << size << "\n";
	}

	delete tcp;

	return 0;
}