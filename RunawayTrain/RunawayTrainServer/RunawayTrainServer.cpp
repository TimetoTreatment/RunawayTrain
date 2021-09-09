#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <thread>
#include <opencv2/opencv.hpp>
#include "../Utility/TCP.h"
#include "Keyboard.h"

using namespace std;
using namespace chrono_literals;
using namespace cv;


int main()
{
	Keyboard key;
	TCP* network = new TCP("9510");
	std::vector<uchar> imageEncoded;
	int size = 0;

	for (bool exit = false; !exit;)
	{
		bool imageReceived = false;

		switch (network->WaitEvent(0))
		{
		case TCP::WaitEventType::NEWCLIENT:
			network->AddClient();

			break;

		case TCP::WaitEventType::MESSAGE:

			if (network->ReadMsg() == "START")
			{
				std::string str = network->ReadMsg();

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

				const char* data = network->ReadData(size);

				imageEncoded.assign(data, data + size);

				Mat image = imdecode(imageEncoded, ImreadModes::IMREAD_COLOR);
				Mat imageResize;

				resize(image, imageResize, Size(1920, 1080));

				imshow("mat", imageResize);

				imageReceived = true;
			}
			cout << "image Received: " << imageReceived << '\n';
			break;

		case TCP::WaitEventType::DISCONNECT:
			network->CloseClient();
			break;

		}


		


		if (waitKey(1) == 'q')
			break;

		std::cout << size << "\n";

		if (key.LeftPressed())
			cout << "L";

		if (key.RightPressed())
			cout << "R";

		if (key.UpPressed())
		{
			network->Send("UP", 5);



		}

		if (key.DownPressed())
			cout << "S";
	}


	delete network;

	return 0;
}