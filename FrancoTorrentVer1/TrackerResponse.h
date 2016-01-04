#pragma once

#include "OrderedMap.h"
#include "Bencoding.h"
#include <sstream>
#include <winsock.h>
#include "TcpPeers.h"
#include <string>

class TrackerResponse
{
private:
	Bencoding bencoder;
	Value v;
public:

	TrackerResponse(char * buffer, int bufLen);

	Value GetResponse();

	OrderedMap<std::string, unsigned short> DecodePeers(char * buffer);
};