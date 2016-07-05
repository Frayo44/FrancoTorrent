
#include <winsock.h>
#include <stdio.h>
#include <fcntl.h>
#include <windows.h>
#include <iostream>
#include <fcntl.h>
class SelectSocket
{
	

		/* Socket has been created and connected to the other party */
		
public:


	int result;
	fd_set readset;
	SOCKET socket_fd;
	struct timeval tv, tv1;
	
	


	SelectSocket(int type, int protocol)
	{
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		WSADATA wsaData;
		if (WSAStartup(0x0202, &wsaData) != 0)
		{
			throw "Unable to load WinSock DLL";
		}

		if ((socket_fd = socket(AF_INET, type, protocol)) == INVALID_SOCKET)
		{
			throw "Connect failed (connect())";
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
			if (connect(socket_fd, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
			{
				return false;
			}
		}
		catch (const std::exception& ignored) {}
		return true;
	}

	int Recv(char * buffer, int length)
	{
		memcpy(&tv1, &tv, sizeof(tv));
		
			
		/* The socket_fd has data available to be read */
		int result = recv(socket_fd, buffer, length, 0);
		if (result == 0) {
			/* This means the other side closed the socket */
			closesocket(socket_fd);
			return -1;
		}
		else {
			return result;
		}
		return 1;

	}


	int Recvv(char * buffer, int length)
	{
		memcpy(&tv1, &tv, sizeof(tv));
		/* Call select() */
		do {
			FD_ZERO(&readset);
			FD_SET(socket_fd, &readset);
			result = select(socket_fd + 1, &readset, NULL, NULL, &tv1);
		} while (result == -1 && errno == EINTR);

		if (result > 0) {
			if (FD_ISSET(socket_fd, &readset)) {
				/* The socket_fd has data available to be read */
				int result = recv(socket_fd, buffer, length, 0);
				if (result == 0) {
					/* This means the other side closed the socket */
					closesocket(socket_fd);
					return -1;
				}
				else {
					return result;
				}
				return 1;
			}
		}
		else if (result < 0) {
			/* An error ocurred, just print it to stdout */
			throw ("Error on select(): %s\", strerror(errno)");
		//	return -1;
		} 

		return -1;
	}

	int RecvBySize(char * buffer, int size)
	{
		int recieved = 0;
		const int BLOCK_REQUEST_SIZE = 1500;

		do
		{
			int rtn;
			memcpy(&tv1, &tv, sizeof(tv));

			
					/* The socket_fd has data available to be read */
					int result = recv(socket_fd, buffer + recieved, BLOCK_REQUEST_SIZE, 0);
					if (result == 0) {
						/* This means the other side closed the socket */
						closesocket(socket_fd);
						return -1;
					}
					else {
						recieved += result;
					}
					//return 1;
				
		
		} while (recieved < size);

		return -1;
	}

	bool Send(const char *buffer, int bufferLen)
	{
		if (send(socket_fd, (const char *)buffer, bufferLen, 0) < 0) {
			//throw ("Send failed (send())");
			return false;
		}
	}

	int Disconnect()
	{
		closesocket(socket_fd);
		WSACleanup();
		
		return 0;
	}
		
};