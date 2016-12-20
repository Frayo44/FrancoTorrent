#include "HTTPTracker.h"
#include "BitArray.h"
#include <vector>
void main(void)
{
	try
	{
		HTTPTracker tracker("toorrrent.torrent");
	}
	catch (const char* msg)
	{
		std::clog << msg << std::endl;
	}

	getchar();
}

