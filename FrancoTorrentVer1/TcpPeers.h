#pragma once

#include <string>
#include "OrderedMap.h"
#include "Socket.h"
#include "BittorrRequest.h"

class TcpPeers
{
private:
	OrderedMap<std::string, unsigned short> peers;
	Socket * sock;
	int size;
public:
	TcpPeers(OrderedMap<std::string, unsigned short> peers, std::string infoHash)
	{
		BittorrRequest bitRequest;

		bitRequest.HandShake(peers.GetKeyByIndex(3), peers.GetValueByIndex(3), infoHash);

		/*size = 0;
		this->peers = peers;
		sock = new Socket(SOCK_STREAM, IPPROTO_TCP);
		Connect(peers.GetKeyByIndex(4), peers.GetValueByIndex(4));

		char * buffer = new char[100];

		*buffer = ((char)19);
		std::string peerId = infoHash + "ABCDEFGHIJKLMNOPQRST";
		std::string packet = "BitTorrent protocol";// +infoHash + peerId;

		int i;
		for (i = 1; i < packet.size(); i++)
		{
			*(buffer + i) = packet.at(i - 1);
		}

	
		buffer[i] = packet.at(packet.size() - 1);

		*(buffer + i + 1) = (char)00;
		*(buffer + i + 2) = (char)00;
		*(buffer + i + 3) = (char)00;
		*(buffer + i + 4) = (char)00;
		*(buffer + i + 5) = (char)00;
		*(buffer + i + 6) = '\x10';
		*(buffer + i + 7) = (char)00;
		*(buffer + i + 8) = (char)05;
		
		i += 9;

		for (int a = 0; a < peerId.size(); a++)
		{
			*(buffer + i) = peerId.at(a);
			i++;
		}
		
		size = i;

		Send((const char *) buffer); */

	}

	void Connect(std::string ip, int port)
	{
		sock->Connect(ip, port);
	}

	void Send(const char * packet)
	{

		sock->Send(packet, size);
	}
};