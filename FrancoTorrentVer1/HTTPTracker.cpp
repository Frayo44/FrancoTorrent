#include "HTTPTracker.h"

struct Value;

HTTPTracker::HTTPTracker(std::string path)
{
	File myFile(path);

	Bencoding bencoder;

	// Decode the Bittorent File, stores dictionary inside it.
	Value v = bencoder.Decode(myFile.GetConetnt(), myFile.GetSize());
	if (v.dataType == ValueType::UNINITIALIZED)
		throw "Error while decoding Torrent file!";
	BuildURI(bencoder);

	std::string announceUrl = (*bencoder.tree.dictionary.GetValueByKey("announce")).text;

	std::string host = GetHost(announceUrl);

	BuildHeaders(host);

	httpRequest.Connect(host);

	httpRequest.Send();

	char * buffer = new char[200];
	int recieved = httpRequest.Recv(buffer, 200, 200);
	*(buffer + recieved) = '\0';

	TrackerResponse trackerResponse(buffer, recieved);
	
	Value v1 = (Value) trackerResponse.GetResponse();
	if (v1.dataType == ValueType::UNINITIALIZED)
		throw "Error while getting tracker response!";
	std::string s = (*v1.dictionary.GetValueByKey("peers")).text;
	
	OrderedMap<std::string, unsigned short> peers = trackerResponse.DecodePeers((char *)s.c_str());

	TcpPeers tcpPeers(peers, GetSha1(bencoder), bencoder);
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
	httpRequest.SetUri("info_hash", urlencode(GetSha1(bencoder)));
	httpRequest.SetUri("peer_id", "ADCDEFGHIJKLMNOPQRST");
	//httpRequest.SetUri("ip", "255.255.255.255");
	httpRequest.SetUri("port", "20383");
	httpRequest.SetUri("uploaded", "0");
	httpRequest.SetUri("downloaded", "0");

	int totalSumPieces = 0;
	std::vector<Value> files = (*(*bencoder.tree.dictionary.GetValueByKey("info")).dictionary.GetValueByKey("files")).list;

	for (std::size_t i = 0; i < files.size(); i++)
	{
		totalSumPieces += (*files.at(i).dictionary.GetValueByKey("length")).integer;
	}
	
	httpRequest.SetUri("left", std::to_string(totalSumPieces));
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

	std::string str(encoded.begin(), encoded.end());

	sha1::calc((const char *)str.c_str(), encoded.size(), hash); // 10 is the length of the string

	std::string hashedString(hash, hash + sizeof(hash));
	//std::string str1 = urlencode(hashedString);

	//std::cout << "str1: Hashed:! " << str1 << std::endl;

	return hashedString;
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