#pragma once

#include "HTTPRequest.h"
#include "Bencoding.h"

class HTTPTracker
{
public:
	HTTPTracker(void) {};

	bool BuildPacket() 
	{
		// Set method to get
		httpRequest.SetMethod(HTTPMethod::GET);

		httpRequest.SetUri("...");

		httpRequest.SetHeader("Host", "");

		httpRequest.SetHeader("user-Agent", "fTorrent");

		httpRequest.SetHeader("Accept-Encoding", "gzip");

		httpRequest.SetHeader("Connection", "close");

		return true;
	};
private:
	HTTPRequest httpRequest;
	Bencoding bencoding;
};