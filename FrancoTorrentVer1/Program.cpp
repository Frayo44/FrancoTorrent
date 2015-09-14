

#include "Bencoding.h"
#include <fstream>
#include <iostream>
#include "HTTPClient.h"
//typedef unsigned char TByte;

void main(void) {


	// ifstream -> Only for *read* a file
	std::ifstream inFile;
	// represents the size of the object
	size_t size = 0; // here
	// in -> For reading, binary -> read binary, ate -> ???
	inFile.open("MyTorrent.torrent", std::ios::in | std::ios::binary | std::ios::ate);
	char* oData = 0;

	inFile.seekg(0, std::ios::end); // set the pointer to the end
	size = inFile.tellg(); // get the length of the file
	std::cout << "Size of file: " << size;
	inFile.seekg(0, std::ios::beg); // set the pointer to the beginning

	oData = new char[size + 1]; //  for the '\0'
	inFile.read(oData, size);
	oData[size] = '\0'; // set '\0' 
	std::cout << " oData size: " << strlen(oData) << "\n";

	//char * ccc = "i540e";
	//	int length = strlen(oData);
	Bencoding bencoder(oData, size);

	delete[] oData;

	HTTPClient httpClient(bencoder);



	getchar();
}