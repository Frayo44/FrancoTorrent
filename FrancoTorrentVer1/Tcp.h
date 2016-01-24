#pragma once

#include "SelectSocket.h"

class Tcp
{
public:
	Tcp()
	{
		this->socket = new SelectSocket(SOCK_STREAM, IPPROTO_TCP);
	}

	virtual bool Connect(std::string ip, int port)
	{
		return socket->Connect(ip, port);
	}

	virtual bool Send(char * buffer, int bufferLen)
	{
		return socket->Send(buffer, bufferLen);
	}

	virtual void Disconnect()
	{
		//socket->DisConnect();
	}

	virtual int Recv(char * buffer, int bufferLen, int toRecieve)
	{
		//int i = socket->Recieve(buffer, bufferLen, toRecieve);
		int i = socket->Recv(buffer, bufferLen);
		return i;
	}

	virtual int RecvBySize(char * buffer, int bufferLen, int toRecieve)
	{
		//int i = socket->Recieve(buffer, bufferLen, toRecieve);
		int i = socket->RecvBySize(buffer, bufferLen);
		return i;
	}

private:
	SelectSocket * socket;
};