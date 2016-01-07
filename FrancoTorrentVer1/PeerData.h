#include <iostream>
#include "OrderedMap.h"

struct IndexOffset
{
	int index;
	int from;
	int till;
};

class PeerData
{
private:
	std::string ip;
	int port;
	OrderedMap<IndexOffset, char *> data;
	std::vector<int> piecesToDownload;
public:
	PeerData(std::string ip, int port) : 
		ip(ip), port(port)
	{ }

	std::string GetIP()
	{
		return ip;
	}
	
	int GetPort()
	{
		return port;
	}

};