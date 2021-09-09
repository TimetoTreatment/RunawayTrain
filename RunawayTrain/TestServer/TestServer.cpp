#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include "../Utility/TCP.h"
#include "../Utility/Console.h"
#include "../Utility/Keyboard.h"

using namespace cv;
using namespace std::chrono_literals;


int main()
{
	TCP* networkCamera = new TCP("9510");
	Console* console = Console::Instance();
	Keyboard key;

	std::vector<uchar> imageEncoded;
	int size = 0;
	std::string message;

	for (bool exit = false; !exit;)
	{
		switch (networkCamera->WaitEvent(0))
		{
		case TCP::WaitEventType::NEWCLIENT:
			networkCamera->AddClient();

			break;

		case TCP::WaitEventType::MESSAGE:

			message = networkCamera->ReadMsg();

			if (message == "START")
			{


				std::string str = networkCamera->ReadMsg();

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

				const char* data = networkCamera->ReadData(size);

				imageEncoded.assign(data, data + size);

				Mat image = imdecode(imageEncoded, ImreadModes::IMREAD_COLOR);

				imshow("Cam", image);

				//Mat imageResize;
				//resize(image, imageResize, Size(1920, 1080));
				//imshow("Cam", imageResize);
			}

			break;

		case TCP::WaitEventType::DISCONNECT:
			networkCamera->CloseClient();
			break;

		}

		std::string input = "";

		if (waitKey(1) == 'q')
			break;

		if (key.LeftPressed())
		{
			input += "Left, ";
		}

		if (key.RightPressed())
			input += "Right, ";

		if (key.UpPressed())
			input += "Up, ";

		if (key.DownPressed())
			input += "Down, ";



		if (!input.empty())
		{
			networkCamera->SendMsg("KEYBOARD");


			networkCamera->SendMsg(input);
		}

		static int frame = 0;

		frame++;
	}

	delete networkCamera;

	return 0;
}