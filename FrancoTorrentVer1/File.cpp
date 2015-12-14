#include "File.h"

File::File(std::string path)
{
	//"MyTorrent.torrent"
	outFile.open(path, std::ios::out | std::ios::in | std::ios::binary | std::ios::ate);
	inFile.open(path, std::ios::in | std::ios::binary | std::ios::ate);
	inFile.seekg(0, std::ios::end); // set the pointer to the end
	size = inFile.tellg(); // get the length of the file
	std::cout << "Size of file: " << size;
	inFile.seekg(0, std::ios::beg); // set the pointer to the beginning

	oData = new char[size + 1]; //  for the '\0'
}

void File::WriteToFile(char * toWrite, int fileLength)
{
	outFile.write(toWrite, fileLength);

	//outFile << "Writing this to a file.\n";
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
	outFile.close();
	delete[] oData;
}