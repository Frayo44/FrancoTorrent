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

		if ((sock = socket(AF_INET, type, protocol)) == INVALID_SOCKET)
		{
			throw "Connect failed (connect())";
		}
	}

	void DisConnect()
	{
		shutdown(sock, 1);
		closesocket(sock);
		CleanUp();
	}

	void CleanUp()
	{
		if (WSACleanup() != 0)
		{
			throw "WSAClenUp() Failed";
		}
	}
	
	bool Connect(const std::string &foreignAddress, int foreignPort)
	{
		// TODO: HANDLE CONNECTIONS
		SOCKADDR_IN addr; // The address structure for a TCP socket

		memset(&addr, 0, sizeof(addr));

		addr.sin_family = AF_INET;      // Address family
		addr.sin_port = htons(foreignPort);   // Assign port to this socket

		hostent *host;

		if ((host = gethostbyname(foreignAddress.c_str())) == NULL) 
		{
			return false;
		}


		addr.sin_addr.s_addr = *((unsigned long *)host->h_addr_list[0]);

		//int err = connect(sock, (sockaddr *)&addr, sizeof(addr));
		try { 
		
			if (connect(sock, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
			{
				return false;
			}
	
		}
		catch (const std::exception& e) { /* */ }

		return true;
	}


	void Send(const char *buffer, int bufferLen)
	{
		if (send(sock, (const char *)buffer, bufferLen, 0) < 0) {
			throw ("Send failed (send())");
		}
	}

	int Recieve(char *buffer, int bufferLen, int toRecieve)
	{

		int recieved = 0;
		const int BLOCK_REQUEST_SIZE = 1500;

		do
		{
			int rtn;

			if (toRecieve - recieved <= BLOCK_REQUEST_SIZE)
			{
				rtn = recv(sock, (char *) (buffer + recieved), toRecieve - recieved, 0);
				recieved += rtn;
				return recieved;
			}
			else 
			{
				rtn = recv(sock, (char *)(buffer + recieved), BLOCK_REQUEST_SIZE, 0);
				recieved += rtn;
			}

			if (rtn < 0) 
			{
				return -1;
			}

			

		} while (recieved < toRecieve);

		

		//*(buffer + rtn) = '\0';

		return recieved;
	}


};