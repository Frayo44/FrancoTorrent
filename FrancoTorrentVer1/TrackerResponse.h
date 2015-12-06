
#pragma once

#include "OrderedMap.h"
#include "Bencoding.h"
#include <sstream>
#include <winsock.h>
#include "TcpPeers.h"

class TrackerResponse
{
private:
	Bencoding bencoder;
	Value v;
public:

	TrackerResponse(char * buffer, int bufLen)
	{

		Bencoding bencoderr;

		v = bencoderr.Decode(buffer, bufLen);



		printf(buffer);
	};

	Value GetResponse()
	{
		// TODO: Check for errors in response
		return v;
	}

	OrderedMap<std::string, unsigned short> DecodePeers5(char * buffer)
	{
		OrderedMap<std::string, unsigned short> peers;
		char *buf = buffer;
		int chunks = strlen(buf) / 6;
		int offset = 0;
		unsigned char *ip;
		unsigned short *port;
		int recsDone = 0;

	


		while (recsDone < chunks){
			ip = (unsigned char *)buf + offset;
			port = (unsigned short *)buf + offset + 4;
			peers.Insert(inet_ntoa(*(in_addr*)ip), ntohs(*port));
			printf("\n\n%s - %d\n", inet_ntoa(*(in_addr*)ip), ntohs(*port));
			//offset += 6;
			buf += 6;
			recsDone++;
		}

		return peers;
	}

};