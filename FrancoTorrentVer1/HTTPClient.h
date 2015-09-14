#pragma once

#include "Bencoding.h"
#include <windows.h>
#include <winsock.h>	
#include <iostream>

class HTTPClient
{
public:
	HTTPClient(Bencoding bencoding)
	{
		connectOnPort(80, bencoding);
	}




private:

	SOCKET s;
	WSADATA w;

	bool connectOnPort(int port, Bencoding bencoding)
	{
		int error = WSAStartup(0x0202, &w);   // Fill in WSA info

		if (error)
		{
			return false; //For some reason we couldn't start Winsock
		}

		if (w.wVersion != 0x0202) //Wrong Winsock version?
		{
			WSACleanup();
			return false;
		}

		SOCKADDR_IN addr; // The address structure for a TCP socket

		addr.sin_family = AF_INET;      // Address family
		addr.sin_port = htons(port);   // Assign port to this socket

		//Accept a connection from any IP using INADDR_ANY
		//You could pass inet_addr("0.0.0.0") instead to accomplish the 
		//same thing. If you want only to watch for a connection from a 
		//specific IP, specify that //instead.

		std::string announce = bencoding.tree.dictionary.find("announce")->second.text;
		printf(announce.c_str());
	
		hostent *h = gethostbyname("tracker.mininova.org");

		if (h == NULL)
		{
			return false;
		}

		memcpy(&addr.sin_addr, h->h_addr_list[0], h->h_length);
		
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket

		if (s == INVALID_SOCKET)
		{
			return false; //Don't continue if we couldn't create a //socket!!
		}

		if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			return false;
		} 

		
		char *c = "http://tracker.mininova.org/announce?info_hash=12345678901234567890";



		int iResult = send(s, c, (int)strlen(c), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return false;
		}

		

		char buf[256];

		

		do {

			iResult = recv(s, buf, 512, 0);
			if (iResult > 0)
				printf("Bytes received: %d\n", iResult);
			else if (iResult == 0)
				printf("Connection closed\n");
			else
				printf("recv failed: %d\n", WSAGetLastError());

		} while (iResult > 0);




		//printf("%d", i);

	}
};
