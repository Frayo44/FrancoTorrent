#pragma once 

#include <fstream>
#include <iostream>

class File
{
private:
	std::ifstream inFile;				// For reading a file
	std::ofstream outFile;
	size_t size = 0; 
	char* oData = 0;
public:
	File(std::string path);

	char * GetConetnt();

	int GetSize();

	void Close();

	void WriteToFile(char * toWrite, int fileLength);

};