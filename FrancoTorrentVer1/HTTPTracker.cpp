#include "HTTPTracker.h"


HTTPTracker::HTTPTracker(std::string path)
{
	File myFile(path);

	Bencoding bencoder;

	bencoder.Decode(myFile.GetConetnt(), myFile.GetSize());

	BuildURI(bencoder);

	std::string announceUrl = (*bencoder.tree.dictionary.GetValueByKey("announce")).text;

	std::string url = announceUrl.substr(0, announceUrl.size() - 9);

	std::string host = GetHost(announceUrl);

	BuildHeaders(host);

	char * myPacket = BuildPacket();

	std::cout << std::endl << "My packet: " << myPacket;

	Socket sock(SOCK_STREAM, IPPROTO_TCP);

	sock.Connect(host, 80);

	std::string str(myPacket);
	sock.Send(myPacket, str.size());

	char * buffer = new char[200];
	sock.Recieve(buffer, sizeof(*buffer));

	printf(buffer);
}

std::string HTTPTracker::GetHost(std::string announceUrl)
{
	std::string host = "";

	int counter = 0;
	for (int i = 0; i < announceUrl.size(); i++)
	{
		if (announceUrl.at(i) == '/')
		{
			counter++;
			continue;
		}

		if (counter == 2)
		{
			host.append(&announceUrl.at(i), 1);
		}
	}

	return host;
}

void HTTPTracker::BuildURI(Bencoding &bencoder)
{
	httpRequest.SetUri("info_hash", GetSha1(bencoder));
	httpRequest.SetUri("peer_id", "ABCDEFGHIJKLMNOPQRST");
	//httpRequest.SetUri("ip", "255.255.255.255");
	httpRequest.SetUri("port", "6883");
	httpRequest.SetUri("uploaded", "0");
	httpRequest.SetUri("downloaded", "0");
	httpRequest.SetUri("left", "98765");
	httpRequest.SetUri("event", "started");
}

void HTTPTracker::BuildHeaders(std::string host)
{
	httpRequest.SetMethod(HTTPMethod::GET);
	httpRequest.SetHeader("Host", host);
	httpRequest.SetHeader("user-Agent", "uTorrent/345(109814002)(41202)");
	httpRequest.SetHeader("Accept-Encoding", "gzip");
	httpRequest.SetHeader("Connection", "close");
}

std::string HTTPTracker::GetSha1(Bencoding &bencoder)
{
	TToken encoded = bencoder.Encode("info");

	unsigned char hash[20];
	char hexstring[41];

	sha1::sha11 sha1;

	std::string str(encoded.begin(), encoded.end());

	sha1.calc((const char *)str.c_str(), encoded.size(), hash); // 10 is the length of the string

	std::string hashedString(hash, hash + sizeof(hash));
	std::string str1 = urlencode(hashedString);

	std::cout << "str1: Hashed:! " << str1 << std::endl;

	return str1;
}

std::string HTTPTracker::char2hex(char dec)
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

std::string HTTPTracker::urlencode(const std::string &c)
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

char * HTTPTracker::BuildPacket()
{
	std::vector<char> buffer;

	httpRequest.GetBuffer(buffer);

	int length = buffer.size();

	char * ptrContent = new char[length + 1]; // For null termianated

	int i;

	for (i = 0; i < length; i++)
	{
		*(ptrContent + i) = buffer.at(i);
	}

	*(ptrContent + i) = '\0';

	return ptrContent;
};