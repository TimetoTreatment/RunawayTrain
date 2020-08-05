#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

#define	LED	26
using namespace std;

int main(void)
{
	string input;
	wiringPiSetup();

	pinMode(LED, OUTPUT);

	for (;;)
	{
		cin >> input;

		if (input == "on")
		{
			cout << "on\n";
			digitalWrite(LED, HIGH);
			delay(1000);
		}

		else
		{
			cout << "off\n";
			digitalWrite(LED, LOW);
			delay(1000);
		}
	}

	return 0;
}