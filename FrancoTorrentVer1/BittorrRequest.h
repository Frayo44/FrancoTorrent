#pragma once

#include <vector>
#include "Tcp.h"


class BittorrRequest : public Tcp
{
public:

	void HandShake(std::string ip, int port, std::string infoHash)
	{
		std::vector<char> buffer;
		Tcp::Connect(ip, port);

		std::string protocolStr = "BitTorrent protocol";
		std::string hashAndID = infoHash + "ABCDEFGHIJKLMNOPQRST";
		char protocolStrLen = (char)protocolStr.size();

		buffer.push_back(protocolStrLen);
		
		for (int i = 0; i < protocolStr.size(); i++)
		{
			buffer.push_back(protocolStr.at(i));
		}

		buffer.push_back((char)00);
		buffer.push_back((char)00);
		buffer.push_back((char)00);
		buffer.push_back((char)00);
		buffer.push_back((char)00);
		buffer.push_back('\x10');
		buffer.push_back((char)00);
		buffer.push_back((char)05);

		for (int i = 0; i < hashAndID.size(); i++)
		{
			buffer.push_back(hashAndID.at(i));
		}

		char * buff = new char[buffer.size()];

		for (int i = 0; i < buffer.size(); i++)
		{
			buff[i] = buffer.at(i);
		}

		Tcp::Send(buff, buffer.size());
	}
private:
	
};