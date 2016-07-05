#pragma once

#include <string>
#include "BittorrRequest.h"
#include "BitArray.h"
#include "PieceItem.h"

class Peer
{
private:
	std::string ip, infoHash;
	int port;
	bool isConnected;
	std::vector<unsigned char> bitfield;
	bool finished, firstTime;
	std::vector<PieceItem> pieces;
	int currFileIndex, currOffSet;
	int pieceSize = 0, offset = 0, requestSize = 0, currPieceIndex = 0;
	char * newBuffer;

	int dataToDownaload = 0;

public:
	BittorrRequest * bitRequest;
	 
	Peer(std::string ip, int port, std::string infoHash, std::vector<PieceItem>& pieces) :
		ip(ip), port(port), infoHash(infoHash), pieces(pieces)
	{
	//	file = new 
		isConnected = false;
		bitRequest = new BittorrRequest();
		currFileIndex = 0;
		pieceSize = pieces.at(0).fileSize;
		firstTime = true;
	}

	void Disconnect()
	{
		bitRequest->Disconnect();
		bitRequest = new BittorrRequest();
		isConnected = false;
		firstTime = true;
	}

	bool IsConnected()
	{
		return isConnected;
	}

	bool CreateConnection()
	{
		if (!bitRequest->Connect(ip, port)) 
			return false;

		bitRequest->HandShake(ip, port, infoHash);

	

		//  TODO: Now it's just fixed sizes by number that have been seen in WireShark
	/*	char * buffer2 = new char[1500];
		// BitField
		int recieved2 = bitRequest->Recv(buffer2, 1500, 1500);
		byte bufferg = (char)buffer2[4];
		if (recieved2 > 0 && bufferg == '\x5')
		{
			BitArray bitArray;
			bitfield = bitArray.getBitsArrFromBuffer(buffer2 + 5, recieved2);
			// It's BitField!
		} */

		bitRequest->Interested();

		isConnected = true;
		return true;
	}

	void firstTimee()
	{
		if (firstTime)
		{
			char * buffer = new char[58];
			// Handshake (Extended)
			//int recieved = bitRequest->Recv(buffer, 68, 68);
			int recieved = bitRequest->Recv(buffer, 58, 58);

			if (recieved > 0 && buffer[0] == '\x13')
			{ 
				int i = 0;
				// Handshake recieved
			}

			firstTime = false;
		}

	}

	void decide()
	{
		char buffer[5];
		int recievedLength = bitRequest->Recv(buffer, 5, 5);
		char buffer2[1500];
		int ss = 0;
		int dataRecieved = 0;
		if (recievedLength > 0)
		{
			//recievedLength = 
			switch (buffer[4])
			{
			case '\x1':
				// Unchoke
				//bitRequest->Recv(buffer2, buffToInteger(buffer) - 5, 5);
				break;
			case '\x5':
				if (buffer[0] == '\x0')
					bitRequest->Recv(buffer2, buffToInteger(buffer) - 1, 5);
				else
					bitRequest->Recv(buffer2, 4, 5);
				break;
			case '\x7':
				// Piece
				ss = buffToInteger(buffer) - 1;
				dataRecieved += GetPiece(ss);
				//	bitRequest->Recv(buffer2, buffToInteger(buffer) - 5, 5);
				break;
			case '\x14':
				// Handshake extended recieved
				ss = buffToInteger(buffer) - 1;
				bitRequest->Recv(buffer2, ss, 5);
				//	bitRequest->Recv(buffer2, buffToInteger(buffer) - 5, 5);
				break;
			default:
				//int size = bitRequest->Recv(buffer2, 4, 5);
				break;
			}
		}
	}

	bool Listen(PieceItem piece)
	{

		pieceSize = piece.lastFileSize;
		offset = piece.lastOffset;
		currPieceIndex = piece.lastIndex;
		currFileIndex = piece.fileIndex;
		int dataRecieved = 0;
		finished = false;
		bool from = true;
		if (!firstTime)
			if (pieceSize > 16384)
				bitRequest->RequestPiece(piece.lastIndex, offset, 16384);
			else {

				bitRequest->RequestPiece(piece.lastIndex, offset, pieceSize);
			}


			while (dataRecieved < piece.fileSize && !finished)
			{
				if (firstTime)
				{
					firstTimee();
					decide();
					if (pieceSize > 16384)
						bitRequest->RequestPiece(piece.lastIndex, offset, 16384);
					else {

						bitRequest->RequestPiece(piece.lastIndex, offset, pieceSize);
					}
					continue;
				}


				char buffer[5];
				int recievedLength = bitRequest->Recv(buffer, 5, 5);
				char buffer2[1500];
				int ss = 0;

				if (recievedLength > 0)
				{
					//recievedLength = 
					switch (buffer[4])
					{
					case '\x1':
						// Unchoke
						//bitRequest->Recv(buffer2, buffToInteger(buffer) - 5, 5);
						break;
					case '\x5':
						if (buffer[0] == '\x0')
							bitRequest->Recv(buffer2, buffToInteger(buffer) - 1, 5);
						else
							bitRequest->Recv(buffer2, 4, 5);
						break;
					case '\x7':
						// Piece
						ss = buffToInteger(buffer) - 1;
						if (ss > 1000000 || ss <= 0)
						{
							finished = true;
							from = false;
						}
						//bitRequest->Recv(buffer2, 1500, 1500);
						else
							dataRecieved += GetPiece(ss);
						//	bitRequest->Recv(buffer2, buffToInteger(buffer) - 5, 5);
						break;
					case '\x14':
						// Piece
						ss = buffToInteger(buffer) - 1;
						bitRequest->Recv(buffer2, ss, 5);
						//	bitRequest->Recv(buffer2, buffToInteger(buffer) - 5, 5);
						break;
					default:
						//int size = bitRequest->Recv(buffer2, 4, 5);
						break;
					}
				}

			}

			if (dataRecieved >= piece.fileSize || (finished && from))
				return true;

			return false;
	}

	int GetPiece(int size)
	{
		newBuffer = new char[size];
		bitRequest->RecvBySize(newBuffer, size);
		//newBuffer += 8;
		File file("./Music/" + pieces.at(currFileIndex).fileName);
		file.WriteToFile(newBuffer + 8, size - 8);
		file.Close();

		delete[] newBuffer;

		pieceSize -= (size - 8);
		std::cout << "Downloading File Index: " << currFileIndex << " More: " << pieceSize << std::endl;

		if (pieceSize <= 0)
		{
			finished = true;
			pieces.at(currFileIndex).setFinish(true);
			return 0;
			//currFileIndex++;
			//pieceSize = pieces.at(currFileIndex).fileSize;
		}

		pieces.at(currFileIndex).lastIndex = currPieceIndex;
		pieces.at(currFileIndex).lastOffset = offset;
		pieces.at(currFileIndex).lastFileSize = pieceSize;
		offset += size - 8;

		if (pieceSize > 16384)
		{
			if (1048576 - offset < 16384)
			{
				requestSize = 1048576 - offset;
				if (requestSize == 0)
				{
					bitRequest->HavePiece(currPieceIndex);
					offset = 0;
					currPieceIndex++;
					if (pieceSize > 16384)
					{
						requestSize = 16384;
					}
					else {
						requestSize = pieceSize;
					}
				}
			}
			else {
				requestSize = 16384;
			}

		}
		else {
			requestSize = pieceSize;
		}

		bitRequest->RequestPiece(currPieceIndex, offset, requestSize);
		return size - 8;
	}

	int buffToInteger(char * buffer)
	{
		int a = int((unsigned char)(buffer[0]) << 24 |
			(unsigned char)(buffer[1]) << 16 |
			(unsigned char)(buffer[2]) << 8 |
			(unsigned char)(buffer[3]));
		return a;
	}

	// TODO: Create this function.. with bitArray class.
	bool HasPeace(int pieceIndex)
	{
		return true;
	}
};