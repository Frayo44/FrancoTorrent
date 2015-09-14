//class HTTPPacket {};
#pragma once

#include <map>
#include <string>
#include <vector>
 
enum HTTPMethod {
	GET, POST, DELETE, PUT, UPDATE, HEAD, OPTIONS
};

class HTTPRequest // 1.1
{
public:
	HTTPRequest(void);

	bool GetBuffer(std::vector<char>& buffer);

	void PushChars(std::vector<char>& buffer, const char * text);


	void SetMethod(HTTPMethod httpMethod);

	void SetUri(std::string uri);
	
	void SetHeader(std::string key, std::string value);
	

	void RemoveHeader(std::string key);

	HTTPMethod GetMethod(HTTPMethod httpMethod);

	void SetBody(char * body);


private:
	HTTPMethod method;
	std::string uri;
	std::map<std::string, std::string>	headers;
	char * body;
};
