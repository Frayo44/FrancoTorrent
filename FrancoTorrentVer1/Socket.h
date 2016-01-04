#pragma once

#include <windows.h>
#include <winsock.h>	
#include <iostream>


class Socket
{
private:
	SOCKET sock;
public:
	Socket(int type, int protocol);

	void DisConnect();

	void CleanUp();
	
	bool Connect(const std::string &foreignAddress, int foreignPort);

	void Send(const char *buffer, int bufferLen);

	int Recieve(char *buffer, int bufferLen, int toRecieve);
};