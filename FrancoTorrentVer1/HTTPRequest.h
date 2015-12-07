#pragma once

#include <map>
#include <string>
#include <vector>
#include "OrderedMap.h"
#include "Tcp.h"
 
enum HTTPMethod {
	GET, POST, PUT, UPDATE, HEAD, OPTIONS
};

class HTTPRequest : public Tcp // 1.1
{
public:
	bool GetBuffer(std::vector<char>& buffer);

	void PushChars(std::vector<char>& buffer, const char * text);

	void SetMethod(HTTPMethod httpMethod);

	void SetUri(std::string uri);
	
	void SetHeader(std::string key, std::string value);

	void SetUri(std::string key, std::string value);
	
	void RemoveHeader(std::string key);

	HTTPMethod GetMethod(HTTPMethod httpMethod);

	void ClearHeader();

	void SetBody(char * body);

	void Connect(std::string ip)
	{
		Tcp::Connect(ip, 80);
	}

	void Send()
	{
		std::vector<char> buffer;
		GetBuffer(buffer);

		int length = buffer.size();

		char * ptrContent = new char[length + 1]; // For null termianated

		int i;

		for (i = 0; i < length; i++)
		{
			*(ptrContent + i) = buffer.at(i);
		}

		*(ptrContent + i) = '\0';

		std::string myPacket(ptrContent); // for getting size
		Tcp::Send(ptrContent, myPacket.size());
	}


private:
	HTTPMethod method;
	std::string uri;
	OrderedMap<std::string, std::string>	headers;
	OrderedMap<std::string, std::string>	uris;
	char * body = NULL;
};
