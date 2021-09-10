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
	TCP* network = new TCP("9510");
	Console* console = Console::Instance();
	Keyboard key;

	std::vector<uchar> imageEncoded;
	int size = 0;
	std::string message;

	for (bool exit = false; !exit;)
	{
		switch (network->WaitEvent(0))
		{
		case TCP::WaitEventType::NEWCLIENT:
			network->AddClient();

			break;

		case TCP::WaitEventType::MESSAGE:

			message = network->ReadMsg();

			if (message == "START")
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

				imshow("Cam", image);
			}

			break;

		case TCP::WaitEventType::DISCONNECT:
			network->CloseClient();
			break;

		}

		static bool up = false;
		static bool down = false;
		static bool left = false;
		static bool right = false;
		static bool prevUp = false;
		static bool prevDown = false;
		static bool prevLeft = false;
		static bool prevRight = false;

		if (waitKey(1) == 27)
		{
			network->SendMsg("EXIT");
			break;
		}

		if (key.UpPressed())
			up = true;
		else
			up = false;

		if (key.DownPressed())
			down = true;
		else
			down = false;

		if (key.LeftPressed())
			left = true;
		else
			left = false;

		if (key.RightPressed())
			right = true;
		else
			right = false;

		if (up != prevUp)
		{
			if (up == true)
				network->SendMsg("UP_ON");
			else
				network->SendMsg("UP_OFF");
		}

		if (down != prevDown)
		{
			if (down == true)
				network->SendMsg("DOWN_ON");
			else
				network->SendMsg("DOWN_OFF");
		}

		if (left != prevLeft)
		{
			if (left == true)
				network->SendMsg("LEFT_ON");
			else
				network->SendMsg("LEFT_OFF");
		}

		if (right != prevRight)
		{
			if (right == true)
				network->SendMsg("RIGHT_ON");
			else
				network->SendMsg("RIGHT_OFF");
		}

		prevUp = up;
		prevDown = down;
		prevLeft = left;
		prevRight = right;

		static int frame = 0;
		int xStart = 85;
		console->Clear(xStart, 36, 40, 10);
		console->Draw("* Revceived Data Size : " + std::to_string(size), xStart, 36);
		console->Draw("* Total Frame : " + std::to_string(frame), xStart, 38);
		console->Draw("* Keyboard Pressed", xStart, 40);

		if (up)
			console->Draw("        U", xStart, 42);
		if (right)
			console->Draw("          R", xStart, 43);
		if (left)
			console->Draw("      L", xStart, 43);
		if (down)
			console->Draw("        D", xStart, 44);

		frame++;
	}

	delete network;

	return 0;
}