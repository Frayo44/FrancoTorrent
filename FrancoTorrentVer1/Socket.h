#pragma once

#include <windows.h>
#include <winsock.h>	
#include <iostream>


class Socket
{
private:
	SOCKET sock;

public:

	Socket(int type, int protocol)
	{
		WSADATA wsaData;
		if (WSAStartup(0x0202, &wsaData) != 0)
		{
			throw "Unable to load WinSock DLL";
		}


		sock = socket(AF_INET, type, protocol);
	}

	void CleanUp()
	{
		if (WSACleanup() != 0)
		{
			throw "WSAClenUp() Failed";
		}
	}
	
	void Connect(const std::string &foreignAddress, int foreignPort)
	{

		SOCKADDR_IN addr; // The address structure for a TCP socket

		memset(&addr, 0, sizeof(addr));

		addr.sin_family = AF_INET;      // Address family
		addr.sin_port = htons(foreignPort);   // Assign port to this socket

		hostent *host;

		if ((host = gethostbyname(foreignAddress.c_str())) == NULL) 
		{
			// strerror() will not work for gethostbyname() and hstrerror() 
			// is supposedly obsolete
			throw "Failed to resolve name (gethostbyname())";
		}


		addr.sin_addr.s_addr = *((unsigned long *)host->h_addr_list[0]);
		if (sock == INVALID_SOCKET)
		{
			int i = 5;
		}

		//int err = connect(sock, (sockaddr *)&addr, sizeof(addr));
		if (connect(sock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			throw "Connect failed (connect())";
		}
	}


	void Send(const char *buffer, int bufferLen)
	{
		if (send(sock, (const char *)buffer, bufferLen, 0) < 0) {
			throw ("Send failed (send())");
		}
	}

	int Recieve(void *buffer, int bufferLen)
	{
		int rtn;
		if ((rtn = recv(sock, (char *) buffer, bufferLen, 0)) < 0) {
			throw ("Received failed (recv())", true);
		}

		return rtn;
	}


};