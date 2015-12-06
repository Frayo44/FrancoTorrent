#include "Bencoding.h"
#include "sha1.h"
Bencoding::Bencoding() { }

// Decoding

Value Bencoding::Decode(const TByte *encoded, int length)
{
	TByte *content = (TByte *)encoded;

	std::vector<TToken> tokens;

	bool isSuccess = true;

	try
	{
		Tokenize(content, tokens, length, 0);
		this->tree = FillTree(tokens);
		if (tokens.size() != 0)
			throw "Not all file got formated!";
	}
	catch (...)
	{
		isSuccess = false;
	}

	//Value v = * this->tree.dictionary.getValueByIndex(0);
	//std::string  s = this->tree.dictionary.getKeyByIndex(4);
	std::cout << "Success?: " << isSuccess << std::endl;


	/*unsigned char hash[20];
	char hexstring[41];


	sha1::sha11 sha1;

	sha1.calc("1 1", 3, hash); // 10 is the length of the string
	sha1.toHexString(hash, hexstring);

	int i = 7; */

	return tree;
}


Value Bencoding::FillTree(std::vector<TToken>& tokens)
{
	// Check if tokens list is empty, and if it's not a string 
	if (tokens.size() == 0 || tokens[0].size() != 1)
		throw 456;

	auto currentToken = tokens[0][0];

	if (currentToken == 'i')
		return decodeInt(tokens);
	else if (currentToken == 's')
		return decodeString(tokens);
	else if (currentToken == 'l')
		return decodeList(tokens);
	else if (currentToken == 'd')
		return decodeDict(tokens);
	else
		throw 123;
}

Value Bencoding::decodeInt(std::vector<TToken>& tokens)
{
	assert(tokens[0][0] == 'i');

	if (tokens.size() < 3)
		throw "Unexpected format";
	if (tokens[2][0] != 'e')
		throw "Unexpected format";


	auto number = std::string(tokens[1].begin(), tokens[1].end());
	
	if (!is_number(number))
		throw "Unexpected format";

	auto v = Value(atoi(number.c_str()));

	tokens.erase(tokens.begin()); // remove "i"
	tokens.erase(tokens.begin()); // remove the number
	tokens.erase(tokens.begin()); // remove "e"

	return v;
}

bool Bencoding::is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

Value Bencoding::decodeString(std::vector<TToken>& tokens)
{
	assert(tokens[0][0] == 's');

	auto text = std::string(tokens[1].begin(), tokens[1].end());
	auto v = Value(text);

	tokens.erase(tokens.begin()); // remove "s"
	tokens.erase(tokens.begin()); // remove the string token

	return v;
}

Value Bencoding::decodeList(std::vector<TToken>& tokens)
{
	std::vector<Value> list;

	

	tokens.erase(tokens.begin()); // remove "l" (or "d" when it has been called from decodeDict)

	if (tokens.size() <= 0)
		throw "Unexpected format found";

	while (!(tokens[0][0] == 'e' && tokens[0].size() == 1))
	{
		list.push_back(FillTree(tokens));
		if (tokens.size() <= 0) 
			throw "Unexpected format found";
	} 

	tokens.erase(tokens.begin()); // remove "e"

	return Value(list);
}

Value Bencoding::decodeDict(std::vector<TToken>& tokens)
{
	//std::map<std::string, Value> dict;
	OrderedMap<std::string, Value *> dict;
	auto tmpList = decodeList(tokens);

	for (auto i = tmpList.list.begin(); i != tmpList.list.end(); i += 2)
	{

		if ((*i).dataType != ValueType::STRING)
			throw "Unexpected file format";

		Value * v1 = new Value;

		*v1 = (Value) *(i + 1);

		dict.Insert((*i).text, v1);
     	//dict[(*i).text] = *(i + 1);
	}

	return Value(dict);
}

void Bencoding::Tokenize(TByte *encoded, std::vector<TToken>& tokens, int length, int counter)
{
	//length = length;

	if (counter >= length)
		return;

	//Parse of benconding symbols

	if (*encoded == 'i' || *encoded == 'd' || *encoded == 'l' || *encoded == 'e') {
		TToken t;
		t.push_back(*encoded);

		tokens.push_back(t);
		counter++;
		Tokenize(++encoded, tokens, length, counter);
		return;
	}

	// Parse of strings or plain integers

	bool is_integer = false; // and not string length;	
	bool is_negative = false;

	ValueType type = ValueType::UNINITIALIZED;

	std::vector<TByte> digits;

	if (*encoded == '-') {
		is_negative = true;

		digits.push_back(*encoded);
		counter++;
		encoded++;
	}

	size_t last_digit_index = 0;

	while (isdigit(encoded[last_digit_index])) {
		digits.push_back(encoded[last_digit_index]);
		last_digit_index++;
	}

	if (encoded[last_digit_index] == ':' && digits.size() > 0) {
		// The number digits represents the string length
		is_integer = false;
		type = ValueType::STRING;

		if (is_negative) {
			throw "Number before a string cannot be negative";
		}
	}
	else if (encoded[last_digit_index] == 'e')
	{
		// A plain integer
		is_integer = true;
		type = ValueType::INTEGER;
	}
	else
	{
		type = ValueType::UNINITIALIZED;
	}


	switch (type)
	{
	case ValueType::INTEGER:
	{
		tokens.push_back(digits);
		encoded = encoded + last_digit_index;
		counter += last_digit_index;
		Tokenize(encoded, tokens, length, counter);
		return;

		break;
	}
	case ValueType::STRING:
	{
		//tokens.push_back("s");
		TToken stringDec;
		stringDec.push_back((char)'s');

		encoded++;
		counter++;

		tokens.push_back(stringDec);

		std::string str(digits.begin(), digits.end());
		int strLength = atoi(str.c_str());
		TToken finalStr;



		size_t i = 0;
		encoded = encoded + last_digit_index;
		counter += last_digit_index;
		for (i = 0; i < strLength; i++)
		{
			finalStr.push_back(*(encoded + i));
		}
		encoded += i;
		counter += i;
		tokens.push_back(finalStr);

		Tokenize(encoded, tokens, length, counter);
		return;
		break;
	}
	default:
	{
		encoded += 1;
		counter += 1;
		
		Tokenize(encoded, tokens, length, counter);
		break;
	}
	}

	// If it's a number, add the digits vector as a token
	

	// TODO: else:error, null:finish


}


// Encoding

TToken Bencoding::Encode(const std::string key)
{
	Value mDict = * tree.dictionary.GetValueByKey(key);

	TToken myContent = FillContent(mDict);

	return myContent;
}

TToken Bencoding::FillContent(Value v)
{

	ValueType valueType = v.dataType;

	if (valueType == ValueType::STRING)
		return EncodeString(v);
	if (valueType == ValueType::INTEGER)
		return EncodeInteger(v);
	if (valueType == ValueType::DICTIONARY)
		return EncodeDictionary(v);
	if (valueType == ValueType::LIST)
		return EncodeList(v);

}

TToken Bencoding::EncodeKey(std::string text)
{

	TToken content;

	int textLength = text.size();

	char * buff = new char[50];
	_itoa_s(textLength, buff, 50, 10);


	for (int i = 0; *(buff + i) != NULL; i++)
	{
		content.push_back(*(buff + i));
	}

	content.push_back(':');

	for (int i = 0; i < text.size(); i++)
	{
		content.push_back(text[i]);
	} 

	return content;
}

TToken Bencoding::EncodeString(Value map)
{
	TToken content;
	std::string text = map.text;
	int textLength = text.size();
	
	char * buff = new char[50];
	_itoa_s(textLength, buff, 50, 10);


	for (int i = 0; *(buff + i) != NULL; i++)
	{
		content.push_back(*(buff + i));
	}

	content.push_back(':');

	for (int i = 0; i < textLength; i++)
	{
		content.push_back(text[i]);
	}

	return content;

}

TToken Bencoding::EncodeInteger(Value map)
{
	TToken content;

	content.push_back('i');

	int number = map.integer;
	char * buff = new char[50];
	_itoa_s(number, buff, 50, 10);

	for (int i = 0; *(buff + i) != NULL; i++)
	{
		content.push_back(*(buff + i));
	}

	content.push_back('e');

	return content;

	//FillContent(content, map);
}

TToken Bencoding::EncodeList(Value map)
{
	TToken content;
	std::vector<Value> vList = map.list;

	content.push_back('l');

	for (int i = 0; i < vList.size(); i++) 
	{
		TToken fConetnt = FillContent(vList.at(i));
		content.insert(content.end(), fConetnt.begin(), fConetnt.end());
	}

	content.push_back('e');

	return content;
}

TToken Bencoding::EncodeDictionary(Value map)
{
	TToken content;
	OrderedMap<std::string, Value *> mDictionary = map.dictionary;

	content.push_back('d');

	for (int i = 0; i < mDictionary.GetSize(); i++)
	{
		TToken keyConetnt = EncodeKey(mDictionary.GetKeyByIndex(i));
		content.insert(content.end(), keyConetnt.begin(), keyConetnt.end());

		TToken valueConetnt = FillContent(*mDictionary.GetValueByIndex(i));
		content.insert(content.end(), valueConetnt.begin(), valueConetnt.end());
	}

	content.push_back('e');

	return content;
}



