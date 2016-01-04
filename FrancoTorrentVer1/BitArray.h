#pragma once

#include <vector>

class BitArray
{
private:
	
public:
	BitArray()
	{
		
	}

	std::vector<unsigned char> getBitsArrFromBuffer(char * buffer, int len)
	{
		std::vector<unsigned char> bitfield;

		for (std::size_t i = 0; i < len; i++)
		{
			char c = *(buffer + i);
			for (int a = 0; a < 8; a++)
			{
				/* Shift right */
				bitfield.push_back((c >> a) & 1);
			}
		}

		return bitfield;
	}



};