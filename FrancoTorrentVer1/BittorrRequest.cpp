#include "BittorrRequest.h"

void BittorrRequest::HandShake(std::string ip, int port, std::string infoHash)
{
	std::vector<char> buffer;
	//Tcp::Connect(ip, port);

	std::string protocolStr = "BitTorrent protocol";
	std::string hashAndID = infoHash + "ABCDEFGHIJKLMNOPQRST";
	char protocolStrLen = (char)protocolStr.size();

	buffer.push_back(protocolStrLen);

	for (int i = 0; i < protocolStr.size(); i++)
	{
		buffer.push_back(protocolStr.at(i));
	}

	buffer.push_back((char)00);
	buffer.push_back((char)00);
	buffer.push_back((char)00);
	buffer.push_back((char)00);
	buffer.push_back((char)00);
	buffer.push_back('\x10');
	buffer.push_back((char)00);
	buffer.push_back((char)05);

	for (std::size_t i = 0; i < hashAndID.size(); i++)
	{
		buffer.push_back(hashAndID.at(i));
	}

	char * buff = new char[buffer.size()];

	for (int i = 0; i < buffer.size(); i++)
	{
		buff[i] = buffer.at(i);
	}

	Tcp::Send(buff, buffer.size());
}

void BittorrRequest::Interested()
{
	std::vector<char> buffer;
	int msglenghth = 1;
	PushInteger(msglenghth, buffer);
	buffer.push_back(2);

	char * buff = new char[buffer.size()];

	for (std::size_t i = 0; i < buffer.size(); i++)
	{
		buff[i] = buffer.at(i);
	}

	Send(buff, buffer.size());
}

void BittorrRequest::HavePiece(int index)
{
	std::vector<char> buffer;
	int msgLen = 5, msgType = 4; // (Have - > 4)

	PushInteger(msgLen, buffer);
	buffer.push_back((char)msgType);
	PushInteger(index, buffer);

	char * buff = new char[buffer.size()];

	for (std::size_t i = 0; i < buffer.size(); i++)
	{
		buff[i] = buffer.at(i);
	}

	Send(buff, buffer.size());
}

void BittorrRequest::RequestPiece(int index, int begin, int length)
{
	std::vector<char> buffer;
	int msgLength = 13, msgType = 6;


	PushInteger(msgLength, buffer);
	buffer.push_back((char)6);
	PushInteger(index, buffer);
	PushInteger(begin, buffer);
	PushInteger(length, buffer);

	char * buff = new char[buffer.size()];

	for (std::size_t i = 0; i < buffer.size(); i++)
	{
		buff[i] = buffer.at(i);
	}

	Send(buff, buffer.size());
}

void BittorrRequest::PushInteger(int integer, std::vector<char> & buffer)
{
	for (int i = 3; i >= 0; i--)
	{
		buffer.push_back(integer >> (i * 8));
	}
}

int BittorrRequest::RecvPiece(int requestedLength, std::string path)
{

	File file(path);
	char * buffer = new char[requestedLength];
	int counter = 1;
	int sumPieces = 0;

	//buffer = new char[1506];
	int recieved = Tcp::Recv(buffer, requestedLength, requestedLength);

	if (recieved > 0)
	{
		//sumPieces += recieved;
		file.WriteToFile(buffer, recieved);
		sumPieces += recieved;
		counter++;
	}

	//delete[] buffer;
	file.Close();

	return requestedLength;
}