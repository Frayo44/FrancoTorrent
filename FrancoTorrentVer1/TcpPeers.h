#pragma once

#include <string>
#include "OrderedMap.h"
#include "Socket.h"
#include "BittorrRequest.h"
#include "Peer.h"
#include "BitArray.h"
#include "Bencoding.h"
#include <thread>
#include <iostream>
#include "PeerData.h"
#include "PieceItem.h"

class TcpPeers
{
private:
	OrderedMap<std::string, unsigned short> peers;
	Socket * sock;
	int size;
	Peer*	peersArr;
	std::vector<Value> files;
	int pieceLength;
	long totalSumPieces;
	int numPieces;
	std::vector<PieceItem> pieces;

public:
	void PeerHandeling()
	{
		// TODO: Crate Class for downloading torrent files and call it from thread, Peer Connetion -> PeerHandeling, Peer Connection Data 

		for (std::size_t i = 0; i < peers.GetSize(); i++)
		{
			//std::thread t(&Peer::CreateConnection, *(peersArr + i));

			//	t.

			/*if ((*(peersArr + i)).IsConnected())
			{
			std::thread t2(&Peer::RecievePeace, *(peersArr + i), 0, 1048576);
			t2.join();
			} */
		}

		//		t1.join();
	}
	bool done = false;

	void PeerCommunication(PeerData &peerData, std::string infoHash)
	{
		

		Peer peer(peerData.GetIP(), peerData.GetPort(), infoHash, pieces);
		bool isConnected = peer.CreateConnection();
		if (isConnected)
		{

			for (size_t i = 0; i < pieces.size(); i++)
			{
				if (!pieces.at(i).isInDownload() && !pieces.at(i).isFinished())
				{
					pieces.at(i).setBegin(true);
					pieces.at(i).setInDownload(true);
					//pieces.at(i).setFinish(peer.Listen(pieces.at(i)));
					int result  = peer.Listen(pieces.at(i));

					if (result == -1)
						return;
					if (result == 1)
						pieces.at(i).setFinish(true);
					else
						pieces.at(i).setFinish(false);
					if (!pieces.at(i).isFinished())
					{						
						pieces.at(i).setInDownload(false);
						peer.Disconnect();
						bool isConnected = peer.CreateConnection();
						if (isConnected && !pieces.at(i).isFinished())
							i--;
						// Download of piece didn't complete, reconnect, and try again!
					}
					else {
						i = -1;
					}
				}
			}
			/*
			for (size_t i = 0; i < pieces.size(); i++)
			{
				if (!pieces.at(i).isFinished())
				{
					pieces.at(i).setBegin(true);
					pieces.at(i).setInDownload(true);
					pieces.at(i).setOnTransfer(true);
					pieces.at(i).setFinish(peer.Listen(pieces.at(i)));
					if (!pieces.at(i).isFinished())
					{
						pieces.at(i).setInDownload(false);
						peer.Disconnect();
						bool isConnected = peer.CreateConnection();
						if (isConnected && !pieces.at(i).isFinished())
							i--;
						// Download of piece didn't complete, reconnect, and try again!
					}
				}
			}*/

			
			//thread.join();
		}
	}

	void fillPiecesIndexes()
	{
		// Block Size
		int blockSize = pieceLength;
		int currIndex = 0;
		int requestIndex = 0;
		bool changed = false;
		for (int i = 0; i < files.size(); i++)
		{

			int fileSize = (*files.at(i).dictionary.GetValueByKey("length")).integer;
			std::string str = (*files.at(i).dictionary.GetValueByKey("path")).list.at(0).text;

			bool go = false;
			if (i > 0)
			{
				int endOffSet = pieces.at(i - 1).endOffSet + (fileSize % 1048576);
				int endIndex = currIndex + fileSize / blockSize;
				if (endOffSet >= 1048576)
				{
					endOffSet = endOffSet % 1048576; endIndex++; go = true;
				}
				PieceItem pieceItem(currIndex, endIndex , pieces.at(i - 1).endOffSet, endOffSet, fileSize, str, i);
				pieces.push_back(pieceItem);
			}
			else {
				PieceItem pieceItem(currIndex, 0 + fileSize / blockSize, 0, 0 + (fileSize % 1048576), fileSize, str, i);
				pieces.push_back(pieceItem);
			}

			currIndex += fileSize / blockSize;
			if (go)
				currIndex++;

		}
		
	}

	TcpPeers(OrderedMap<std::string, unsigned short> peers, std::string infoHash, Bencoding bencoder)
	{
		std::vector<Value> files = (*(*bencoder.tree.dictionary.GetValueByKey("info")).dictionary.GetValueByKey("files")).list;
		this->files = files;
	
		for (std::size_t i = 0; i < files.size(); i++)
		{
			totalSumPieces += (*files.at(i).dictionary.GetValueByKey("length")).integer;
		}

		pieceLength = (*(*bencoder.tree.dictionary.GetValueByKey("info")).dictionary.GetValueByKey("piece length")).integer;


		fillPiecesIndexes();


		numPieces = totalSumPieces / pieceLength;

		std::vector<std::thread> threads;
		//std::thread ttt[num_threads];
		//std::thread t1(task1, "Hello");
		for (std::size_t i = 0; i < peers.GetSize(); i++)
		{
			PeerData pd(peers.GetKeyByIndex(i), peers.GetValueByIndex(i));
			
			threads.push_back(std::thread(&TcpPeers::PeerCommunication,this,  pd, infoHash));
		}

		for (std::size_t i = 0; i < peers.GetSize(); i++)
		{
			threads.at(i).join();
		}

	}

};