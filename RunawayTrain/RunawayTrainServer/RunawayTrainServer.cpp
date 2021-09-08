#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <thread>
#include "../Utility/TCP.h"
#include "Keyboard.h"


using namespace std;
using namespace chrono_literals;





int main()
{
	Keyboard key;
	TCP* network = new TCP("9510");
	string left;

	for (;;)
	{
		if (key.LeftPressed())
			cout << "L";

		if (key.RightPressed())
			cout << "R";

		if (key.UpPressed())
		{
			network->Send("LEFT", 5);



		}

		if (key.DownPressed())
			cout << "S";


	}


	delete network;

	return 0;
}