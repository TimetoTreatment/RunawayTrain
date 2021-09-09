#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include "../Utility/TCP.h"
#include "../Utility/Console.h"

using namespace cv;
using namespace std::chrono_literals;


int main()
{
	TCP* tcp = new TCP("9510");
	Console* console = Console::Instance();

	std::vector<uchar> imageEncoded;
	int size = 0;

	for (bool exit = false; !exit;)
	{
		switch (tcp->WaitEvent(0))
		{
		case TCP::WaitEventType::NEWCLIENT:
			tcp->AddClient();

			break;

		case TCP::WaitEventType::MESSAGE:

			if (tcp->ReadMsg() == "START")
			{
				tcp->Synchronize();

				std::string str = tcp->ReadMsg();

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

				imshow("Cam", image);

				//Mat imageResize;
				//resize(image, imageResize, Size(1920, 1080));
				//imshow("Cam", imageResize);
			}

			break;

		case TCP::WaitEventType::DISCONNECT:
			tcp->CloseClient();
			break;

		}

		if (waitKey(1) == 'q')
			break;

		static int frame = 0;
		console->Draw("* Revceived Data Size : " + std::to_string(size), 4, 2);
		console->Draw("* Total Frame : " + std::to_string(frame), 4, 3);
		frame++;
	}

	delete tcp;

	return 0;
}