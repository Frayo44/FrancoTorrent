#pragma once

#include <map>
#include <string>
#include <vector>
#include "OrderedMap.h"
 
enum HTTPMethod {
	GET, POST, DELETE, PUT, UPDATE, HEAD, OPTIONS
};

class HTTPRequest // 1.1
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


private:
	HTTPMethod method;
	std::string uri;
	OrderedMap<std::string, std::string>	headers;
	OrderedMap<std::string, std::string>	uris;
	char * body = NULL;
};
