#pragma once

#include <string>
#include "BittorrRequest.h"



class Peer
{
private:
	std::string ip, infoHash;
	int port;
	BittorrRequest * bitRequest;
	bool isConnected;
public:

	Peer(){};
	 
	Peer(std::string ip, int port, std::string infoHash) :
		ip(ip), port(port), infoHash(infoHash)
	{
		isConnected = false;
		bitRequest = new BittorrRequest();
	}

	

	bool IsConnected()
	{
		return isConnected;
	}



	void CreateConnection()
	{
		if (!bitRequest->Connect(ip, port)) 
			return;

		bitRequest->HandShake(ip, port, infoHash);
		char * buffer = new char[1500];
		// Handshake (Extended)
		bitRequest->Recv(180, buffer);
		char * buffer2 = new char[1500];
		// BitField
		bitRequest->Recv(19, buffer2);
		bitRequest->Interested();

		isConnected = true;
		//return true;
	}

	bool RecievePeace(int pieceIndex, int pieceLength)
	{
		// TODO: Check if peer has the piece
		int dataRecieved = 0, offSet = 0;
		const int BLOCK_REQUEST_SIZE = 16384;

		while (dataRecieved < pieceLength)
		{
			bitRequest->RequestPiece(pieceIndex, offSet, BLOCK_REQUEST_SIZE);
			int recieved = bitRequest->RecvPiece(BLOCK_REQUEST_SIZE, "work_torrent.mp3");

			dataRecieved += recieved;

			offSet += BLOCK_REQUEST_SIZE;

		}

		bitRequest->HavePiece(pieceIndex);

		return true;

	}
};