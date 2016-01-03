#pragma once

#include <string>
#include "OrderedMap.h"
#include "Socket.h"
#include "BittorrRequest.h"
#include "Peer.h"
#include "BitArray.h"
#include <thread>

class TcpPeers
{
private:
	OrderedMap<std::string, unsigned short> peers;
	Socket * sock;
	int size;
	Peer*	peersArr;
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

	TcpPeers(OrderedMap<std::string, unsigned short> peers, std::string infoHash)
	{


		Peer * peer = new Peer(peers.GetKeyByIndex(1), peers.GetValueByIndex(1), infoHash);

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


		BittorrRequest bitRequest;
		bitRequest.Connect(peers.GetKeyByIndex(1), peers.GetValueByIndex(1));
		bitRequest.HandShake(peers.GetKeyByIndex(1), peers.GetValueByIndex(1), infoHash);
		char * buffer = new char[180];
		bitRequest.Recv(180, buffer);

		char * buffer2 = new char[19];
		// BitField
		std::vector<char> vect = bitRequest.Recv(19, buffer2);


		

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

 		 int i = 0;
		 
		

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