#include "HTTPRequest.h"

bool HTTPRequest::GetBuffer(std::vector<char>& buffer)
{
	buffer.clear();

	switch (method)
	{
	case HTTPMethod::GET:
		PushChars(buffer, "GET");
		break;
	case HTTPMethod::POST:
		PushChars(buffer, "POST");
		break;
	case HTTPMethod::DELETE:
		PushChars(buffer, "DELETE");
		break;
	case HTTPMethod::PUT:
		PushChars(buffer, "PUT");
		break;
	case HTTPMethod::HEAD:
		PushChars(buffer, "HEAD");
		break;
	case HTTPMethod::OPTIONS:
		PushChars(buffer, "OPTIONS");
		break;
	default:
		break;
	}

	PushChars(buffer, " ");

	PushChars(buffer, uri.c_str());

	PushChars(buffer, " ");

	PushChars(buffer, "HTTP/1.1\r\n");

	typedef std::map<std::string, std::string>::iterator it_type;
	for (it_type iterator = headers.begin(); iterator != headers.end(); iterator++) {


		PushChars(buffer, iterator->first.c_str());

		PushChars(buffer, ": ");

		PushChars(buffer, iterator->second.c_str());

		PushChars(buffer, "\r\n");


	}

	PushChars(buffer, body);

	PushChars(buffer, "\r\n");

	return true;
}

void HTTPRequest::PushChars(std::vector<char>& buffer, const char * text)
{
	for (int i = 0; *(text + i) != NULL; i++)
	{
		buffer.push_back(*(text + i));
	}
}

void HTTPRequest::SetMethod(HTTPMethod httpMethod) {
	method = httpMethod;
}


void HTTPRequest::SetUri(std::string uri) {
	this->uri = uri;
}

void HTTPRequest::SetHeader(std::string key, std::string value) {
	headers[key] = value;
}

void HTTPRequest::RemoveHeader(std::string key) {
	headers.erase(key);
}

HTTPMethod HTTPRequest::GetMethod(HTTPMethod httpMethod) {
	return method;
}

void HTTPRequest::SetBody(char * body) {
	this->body = body;
}

