#include "File.h"

File::File(std::string path)
{
	exists_test0(path);
	//"MyTorrent.torrent"
	outFile.open(path, std::ios::out | std::ios::in | std::ios::binary | std::ios::ate);
	inFile.open(path, std::ios::in | std::ios::binary | std::ios::ate);
	inFile.seekg(0, std::ios::end); // set the pointer to the end
	size = inFile.tellg(); // get the length of the file
//	std::cout << "Size of file: " << size;
	inFile.seekg(0, std::ios::beg); // set the pointer to the beginning

	oData = new char[size + 1]; //  for the '\0'
}
bool File::exists_test0(const std::string& name) {
	std::ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		std::ofstream myfile;
		myfile.open(name);
		//myfile << "Writing this to a file.\n";
		myfile.close();
		f.close();
		//ff.close();
		return false;
	}
}


void File::WriteToFile(char * toWrite, int fileLength)
{
	outFile.write(toWrite, fileLength);
	//Close();
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