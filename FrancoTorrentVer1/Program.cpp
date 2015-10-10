

#include "Bencoding.h"
#include "File.h"
#include "HTTPClient.h"
#include "sha1.h"

std::string char2hex(char dec)
{
	char dig1 = (dec & 0xF0) >> 4;
	char dig2 = (dec & 0x0F);
	if (0 <= dig1 && dig1 <= 9) dig1 += 48;    //0,48 in ascii
	if (10 <= dig1 && dig1 <= 15) dig1 += 65 - 10; //A,65 in ascii
	if (0 <= dig2 && dig2 <= 9) dig2 += 48;
	if (10 <= dig2 && dig2 <= 15) dig2 += 65 - 10;

	std::string r;
	r.append(&dig1, 1);
	r.append(&dig2, 1);
	return r;
}

std::string urlencode(const std::string &c)
{

	std::string escaped;
	int max = c.length();
	for (int i = 0; i<max; i++)
	{
		if ((48 <= c[i] && c[i] <= 57) ||//0-9
			(65 <= c[i] && c[i] <= 90) ||//ABC...XYZ
			(97 <= c[i] && c[i] <= 122) || //abc...xyz
			(c[i] == '~' || c[i] == '-' || c[i] == ' ' || c[i] == '.')
			)
		{
			escaped.append(&c[i], 1);
		}
		else
		{
			escaped.append("%");
			escaped.append(char2hex(c[i]));//converts char 255 to string "FF"
		}
	}
	return escaped;
}
void main(void) {

	File myFile("torrenttry1.torrent");

	Bencoding bencoder;
	
	bencoder.Decode(myFile.GetConetnt(), myFile.GetSize());

	TToken encoded;
	encoded = bencoder.Encode();

	unsigned char hash[20];
	char hexstring[41];


	sha1::sha11 sha1;

	std::string str(encoded.begin(), encoded.end());

	sha1.calc((const char *)str.c_str(), encoded.size(), hash); // 10 is the length of the string
	sha1.toHexString(hash, hexstring);


	std::string my_std_string(hash, hash + sizeof(hash));
	std::string str1 = urlencode(my_std_string);

	//std::cout << str1 << std::endl;

	std::cout << std::endl << "str1: Hasheddd!!: " << str1;


	getchar();
}

