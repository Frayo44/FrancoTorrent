#pragma once

#include "HTTPRequest.h"
#include "Bencoding.h"

class HTTPTracker
{
public:
	
	char * BuildPacket(std::string uri, std::string host) 
	{
		std::vector<char> buffer;

		// Set method to get
		httpRequest.SetMethod(HTTPMethod::GET);

		httpRequest.SetUri(uri);

		httpRequest.SetHeader("Host", host);

		httpRequest.SetHeader("user-Agent", "fTorrent");

		httpRequest.SetHeader("Accept-Encoding", "gzip");

		httpRequest.SetHeader("Connection", "close");

		httpRequest.GetBuffer(buffer);

		int length = buffer.size();

		char * ptrContent = new char[length]; // For null termianated

		int i;

		for (i = 0; i < length; i++) 
		{
			*(ptrContent + i) = buffer.at(i);
		}

		return ptrContent;
	};



private:
	HTTPRequest httpRequest;
	Bencoding bencoding;
};