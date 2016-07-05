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
				if (!pieces.at(i).isInDownload())
				{
					pieces.at(i).setBegin(true);
					pieces.at(i).setInDownload(true);
					pieces.at(i).setFinish(peer.Listen(pieces.at(i)));
					if (!pieces.at(i).isFinished())
					{						pieces.at(i).setInDownload(false);
						peer.Disconnect();
						bool isConnected = peer.CreateConnection();
						if (isConnected && !pieces.at(i).isFinished())
							i--;
						// Download of piece didn't complete, reconnect, and try again!
					}


				}
			}
			
			//thread.join();
		}
	}


	void FillPieces(int index, int pieceIndex, int startOffSet)
	{
		int fileSize = (*files.at(pieceIndex).dictionary.GetValueByKey("length")).integer;
		if (index <= 0)
		{
			//PieceItem pieceItem(0, -1, 0, GetRequestSize(fileSize), -1);
			//pieces.push_back(pieceItem);
//			return FillPieces(index + 1, )
		}

		
	}

	int GetRequestSize(int fileSize)
	{
		if (fileSize > 16384)
		{
			return 16384;
		}
		else {
			return fileSize;
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


	int GetPieceIndex(int pieceIndex)
	{
		int until = 0;
		for (int i = 0; i < pieceIndex; i++)
		{
			until += (*files.at(i).dictionary.GetValueByKey("length")).integer;
		}

		return until / pieceLength;
		//	int singlePieceLength = (*files.at(pieceIndex).dictionary.GetValueByKey("length")).integer;
	}
	
	TcpPeers(OrderedMap<std::string, unsigned short> peers, std::string infoHash, Bencoding bencoder)
	{

	
		//Value v = bencoder.SearchForValueByKey("files");
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
		//PeerData pd(peers.GetKeyByIndex(1), peers.GetValueByIndex(1));
	//	std::thread t2(&PeerCommunication, pd, infoHash);
	//	std::thread first(&foo);

	//	Peer peer(peers.GetKeyByIndex(1), peers.GetValueByIndex(1), infoHash);
	//	peer.CreateConnection();

	//	if(peer.HasPeace(0);
		//std::vector<char> testVect;
		//testVect.push_back('a');
		//	BitArray biArr(testVect);
		/*	this->peers = peers;
			peersArr = new Peer[peers.GetSize()];

			for (int i = 0; i < peers.GetSize(); i++)
			{
			Peer * pp = new Peer(peers.GetKeyByIndex(i), peers.GetValueByIndex(i), infoHash);
			*(peersArr + i) = *pp;
			}

			//	std::thread t1(&((peersArr)->Peer::CreateConnection), peersArr[0]);


			std::thread first(&TcpPeers::HandleThreads, *this);
			//	first.join();

			while (true)
			{
			int b = 0;
			}

			first.join();
			*/

		// Makes the main thread wait for the new thread to finish execution, therefore blocks its own execution.
		//t1.join();


		/*BittorrRequest bitRequest;
		bitRequest.Connect(peers.GetKeyByIndex(1), peers.GetValueByIndex(1));
		bitRequest.HandShake(peers.GetKeyByIndex(1), peers.GetValueByIndex(1), infoHash);
		char * buffer = new char[180];
		bitRequest.Recv(buffer, 180, 180);

		char * buffer2 = new char[19];
		// BitField
		int recieved = bitRequest.Recv(buffer2, 19, 19);




		bitRequest.Interested();
		//bitRequest.Recv(buffer, 1500);


		//	bitRequest.RequestPiece(0, 0, 291);
		//	bitRequest.RecvPiece(291, "music.mp3");

		//TODO: Create a peer class, TcpPeers will handle all peers. 
		//TODO: Create a BitArray class for BitField packet
		// TODO: A to-do piece list in TcpPeers class
		// 4056211 + 291

		// Get the first piece

		int index = 0, dataRecieved = 0, offSet = 0;
		const int BLOCK_REQUEST_SIZE = 16384;

		while (dataRecieved < 4056211 + 291)
		{
			bitRequest.RequestPiece(index, offSet, BLOCK_REQUEST_SIZE);
			int recieved = bitRequest.RecvPiece(BLOCK_REQUEST_SIZE, "work_torrent.mp3");

			dataRecieved += recieved;

			offSet += BLOCK_REQUEST_SIZE;

			if (offSet % 1048576 < BLOCK_REQUEST_SIZE - 1)
			{
				bitRequest.HavePiece(index);
				offSet = 0;
				index++;
			}
		}

		int i = 0; */



		/*size = 0;
		this->peers = peers;
		sock = new Socket(SOCK_STREAM, IPPROTO_TCP);
		Connect(peers.GetKeyByIndex(4), peers.GetValueByIndex(4));

		char * buffer = new char[100];

		*buffer = ((char)19);
		std::string peerId = infoHash + "ABCDEFGHIJKLMNOPQRST";
		std::string packet = "BitTorrent protocol";// +infoHash + peerId;

		int i;
		for (i = 1; i < packet.size(); i++)
		{
		*(buffer + i) = packet.at(i - 1);
		}


		buffer[i] = packet.at(packet.size() - 1);

		*(buffer + i + 1) = (char)00;
		*(buffer + i + 2) = (char)00;
		*(buffer + i + 3) = (char)00;
		*(buffer + i + 4) = (char)00;
		*(buffer + i + 5) = (char)00;
		*(buffer + i + 6) = '\x10';
		*(buffer + i + 7) = (char)00;
		*(buffer + i + 8) = (char)05;

		i += 9;

		for (int a = 0; a < peerId.size(); a++)
		{
		*(buffer + i) = peerId.at(a);
		i++;
		}

		size = i;

		Send((const char *) buffer); */

	}


};