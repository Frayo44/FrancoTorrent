#pragma once

#include <vector>
#include "File.h"
#include "Tcp.h"


class BittorrRequest : public Tcp
{
public:

	void HandShake(std::string ip, int port, std::string infoHash);

	void Interested();

	void HavePiece(int index);

	bool RequestPiece(int index, int begin, int length);

	void PushInteger(int integer, std::vector<char> & buffer);

	int RecvPiece(int requestedLength, std::string path);

private:
	
};