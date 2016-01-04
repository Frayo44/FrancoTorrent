#include "TrackerResponse.h"

TrackerResponse::TrackerResponse(char * buffer, int bufLen)
{
	Bencoding bencoderr;
	v = bencoderr.Decode(buffer, bufLen);
	printf(buffer);
}

Value TrackerResponse::GetResponse()
{
	// TODO: Check for errors in response
	return v;
}

OrderedMap<std::string, unsigned short> TrackerResponse::DecodePeers(char * buffer)
{
	OrderedMap<std::string, unsigned short> peers;

	char *buf = buffer;

	int chunks = strlen(buf) / 6;
	int offset = 0;
	unsigned char *ip;
	unsigned short port;
	int recsDone = 0;

	while (recsDone < chunks){
		ip = (unsigned char *)buf + offset;

		port = (unsigned short)(buf[4] << 8) | buf[5];
		//port = (unsigned short) buf + offset + 4;
		peers.Insert(inet_ntoa(*(in_addr*)ip), (port));
		printf("\n\n%s - %d\n", inet_ntoa(*(in_addr*)ip), (port));
		//offset += 6;
		buf += 6;
		recsDone++;
	}

	return peers;
}