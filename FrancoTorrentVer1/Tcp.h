#pragma once

#include "Socket.h"

class Tcp
{
public:
	Tcp()
	{
		this->socket = new Socket(SOCK_STREAM, IPPROTO_TCP);
	}

	virtual bool Connect(std::string ip, int port)
	{
		return socket->Connect(ip, port);
	}

	virtual void Send(char * buffer, int bufferLen)
	{
		socket->Send(buffer, bufferLen);
	}

	virtual void Disconnect()
	{
		socket->DisConnect();
	}

	virtual int Recv(char * buffer, int bufferLen)
	{
		int i = socket->Recieve(buffer, bufferLen);
		return i;
	}

private:
	Socket * socket;
};