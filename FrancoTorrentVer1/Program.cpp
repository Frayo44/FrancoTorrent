
#include "HTTPTracker.h"
#include "BitArray.h"
#include <vector>
void main(void)
{
	try
	{
		HTTPTracker tracker("MyTorrent.torrent");
	}
	catch (const char* msg)
	{
		std::clog << msg << std::endl;
	}
	//std::cout << __cplusplus << std::endl;

	getchar();
}

