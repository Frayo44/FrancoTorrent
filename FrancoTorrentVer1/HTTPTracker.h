#pragma once


#include "HTTPRequest.h"
#include "TrackerResponse.h"
#include "File.h"
#include "Sha1.h"
#include "Bencoding.h"
#include <string>
#include "Socket.h"


#include <cctype>
#include <iomanip>
#include <sstream>

class HTTPTracker
{

private:

	HTTPRequest httpRequest;
	Bencoding bencoding;

public:

	HTTPTracker(std::string path);

	std::string GetHost(std::string announceUrl);

	void BuildURI(Bencoding &bencoder);

	void BuildHeaders(std::string host);

	std::string GetSha1(Bencoding &bencoder);

	std::string char2hex(char dec);

	std::string urlencode(const std::string &c);

	char * BuildPacket(); 

};