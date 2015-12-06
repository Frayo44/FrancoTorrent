#pragma once

#include <string>
#include "OrderedMap.h"
#include "Socket.h"

class TcpPeers
{
private:
	OrderedMap<std::string, unsigned short> peers;

public:
	TcpPeers(OrderedMap<std::string, unsigned short> peers)
	{
		this->peers = peers;

	}

	void Connect(std::string ip, int port)
	{
		Socket sock(SOCK_STREAM, IPPROTO_TCP);
	//	sock.Connect(ip, port);
	}
};