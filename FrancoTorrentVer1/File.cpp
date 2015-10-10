#include "File.h"

File::File(std::string path)
{
	//"MyTorrent.torrent"
	inFile.open(path, std::ios::in | std::ios::binary | std::ios::ate);
	inFile.seekg(0, std::ios::end); // set the pointer to the end
	size = inFile.tellg(); // get the length of the file
	std::cout << "Size of file: " << size;
	inFile.seekg(0, std::ios::beg); // set the pointer to the beginning

	oData = new char[size + 1]; //  for the '\0'
}

char * File::GetConetnt()
{
	inFile.read(oData, size);
	oData[size] = '\0'; // set '\0' 

	return oData;
}

int File::GetSize()
{
	return size;
}

void File::Close()
{
	delete[] oData;
}