#pragma once 

#include <fstream>
#include <iostream>
#include <sys/stat.h>

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

	bool File::exists_test0(const std::string& name);

};