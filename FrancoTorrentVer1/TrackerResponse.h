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
		/*std::string news(buffer, 36);

		std::string ss = news.substr(news.size() - 2);

		const char * bufferr = ss.c_str();

		unsigned short pport3 = (bufferr[0] << 8) | bufferr[1]; 

		//unsigned short port2 = (unsigned short) *bufferr;
		int por = ntohs(pport3);  */
		char *buf = buffer;



		int chunks = strlen(buf) / 6;
		int offset = 0;
		unsigned char *ip;
		unsigned short port;
		int recsDone = 0;

	


		while (recsDone < chunks){
			ip = (unsigned char *) buf + offset;

			port = (unsigned short) (buf[4] << 8) | buf[5];

			//port = (unsigned short) buf + offset + 4;
			peers.Insert(inet_ntoa(*(in_addr*)ip), (port));
			printf("\n\n%s - %d\n", inet_ntoa(*(in_addr*)ip), (port));
			//offset += 6;
			buf += 6;
			recsDone++;
		}

		return peers;
	}

};