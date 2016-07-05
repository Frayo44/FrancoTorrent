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
	
	int recsDone = 0;

	while (recsDone < chunks){
		
		ip = (unsigned char *)buf + offset;
		//unsigned short number = (unsigned short)strtoul(buf[4] + buff[5], NULL, 0);
		unsigned char mostSignifcantChar = buf[4];
		unsigned char lessSignifcantChar = buf[5];
		unsigned short port = (unsigned short)(mostSignifcantChar << 8);
		port = port | lessSignifcantChar;
		//port = (unsigned short) buf + offset + 4; //10118
		peers.Insert(inet_ntoa(*(in_addr*)ip), (port));
		printf("\n\n%s - %d\n", inet_ntoa(*(in_addr*)ip), port);
		
		//offset += 6;
		buf += 6;
		recsDone++;
	}

	return peers;
}