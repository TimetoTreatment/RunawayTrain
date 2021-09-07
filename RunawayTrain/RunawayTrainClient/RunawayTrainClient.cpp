#include <iostream>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>
#include "../Utility/TCPLinux.h"


int main()
{
	cv::VideoCapture videoCapture(0);
	TCP* tcp = new TCP("9510", "192.168.219.102");
	std::string cinBuffer;
	std::string message;
	std::string userName;

	std::cout << "**************************\n";
	std::cout << "* Anonymous Chat Program *\n";
	std::cout << "**************************\n" << std::endl;

	std::cout << "[SERVER] Enter your nickname : ";
	std::cin >> userName;

	tcp->Send("[USERNAME]" + userName);

	cv::Mat frame;

	videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

	for (bool exit = false; !exit;)
	{
		videoCapture >> frame;

		int a = frame.type();

		for (int row=0;row<frame.rows;row++)
		{
			for (int col = 0; col < frame.cols; col++)
			{
				
			}


		}



		// Receive
		switch (tcp->WaitEvent(0))
		{
		case TCP::WaitEventType::MESSAGE:
			std::cout << tcp->ReadMessage();
			break;

		case TCP::WaitEventType::DISCONNECT:
			exit = true;
			break;
		}

		cv::waitKey(1);
	}

	delete tcp;


	return 0;
}