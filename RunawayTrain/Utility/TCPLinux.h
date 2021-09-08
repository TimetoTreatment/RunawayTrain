#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


class TCP
{
public:

	enum class WaitEventType
	{
		NONE,
		NEWCLIENT,
		DISCONNECT,
		MESSAGE,
	};

	enum class SendTo
	{
		EVENT_SOURCE,
		ALL,
		OTHERS,
	};

	TCP(std::string port, std::string targetIP = "0.0.0.0");
	~TCP();

	WaitEventType WaitEvent(int timeoutMicroSecond = -1);

	void Send(const char* message, int size, SendTo sendTo = SendTo::EVENT_SOURCE);

	const char* ReadMessage();
	const char* ReadBuffer(int size);
	std::string ReadSenderID();

	void AddClient();
	void CloseClient();


private:

	bool isServer;

	int mySocket;
	addrinfo mySocketHint;
	int sender = -1;

	std::vector<pollfd> fdArray;
	size_t fdArrayCurrentIndex = 0;

	char* cache = nullptr;
	char* buffer = nullptr;
	int cacheSize = 8192;
	int bufferSize = 8388608;
};
