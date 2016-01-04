#pragma once

#include <string>
#include "BittorrRequest.h"
#include "BitArray.h"


class Peer
{
private:
	std::string ip, infoHash;
	int port;
	BittorrRequest * bitRequest;
	bool isConnected;
	std::vector<unsigned char> bitfield;
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
		char * buffer = new char[182];
		// Handshake (Extended)
		int recieved = bitRequest->Recv(buffer, 182, 182);

		if (recieved > 0 && buffer[0] == '\x13')
		{
			// Handshake recieved
		}

		//  TODO: Now it's just fixed sizes by number that have been seen in WireShark
		char * buffer2 = new char[1500];
		// BitField
		int recieved2 = bitRequest->Recv(buffer2, 1500, 1500);
		byte bufferg = (char)buffer2[4];
		if (recieved2 > 0 && bufferg == '\x5')
		{
			BitArray bitArray;
			bitfield = bitArray.getBitsArrFromBuffer(buffer2 + 5, recieved2);
			// It's BitField!
		}

		bitRequest->Interested();

		isConnected = true;
		//return true;
	}

	bool HasPeace(int pieceIndex)
	{
		return bitfield.at(pieceIndex) == '\x1';
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