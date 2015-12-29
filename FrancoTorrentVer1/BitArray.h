#pragma once

#include <vector>

class BitArray
{
private:
	std::vector<unsigned char> bitfield;
public:
	BitArray(std::vector<char> vect)
	{
		
		// TODO: Replace the vector.. or itertor...
		for (int i = 0; i < vect.size(); i++)
		{
			char c = vect.at(i);
			for (int a = 0; a < 8; a++)
			{
				
				bitfield.push_back((c >> a) & 1);
			}
			
		}
	}



};