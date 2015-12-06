/**
	Bencoding.cpp
	Purpose: Decoding a bittorrent file into readabe variabes.

	@author Yoav Franco
	@version 1.0 27/07/15
*/
#pragma once


#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <cassert>
#include <iostream>

#include "OrderedMap.h"

/**
*	Purpose: Determine the type of variable.
*/
enum class ValueType {
	UNINITIALIZED,
	INTEGER, STRING, LIST, DICTIONARY
};

/**
*	Purpose: Contain all the bittornt file when encoded.
*/
struct Value {
	ValueType						dataType;
	int								integer;
	std::string						text;
	std::vector<Value>				list;
	 
	//std::map<std::string, Value>	dictionary;
	OrderedMap<std::string, Value *>						dictionary;

	Value() :
		dataType(ValueType::UNINITIALIZED)
	{
	}

	Value(int input) :
		dataType(ValueType::INTEGER), integer(input)
	{
	}

	Value(std::string input) :
		dataType(ValueType::STRING), text(input)
	{
	}

	Value(std::vector<Value> input) :
		dataType(ValueType::LIST), list(input)
	{
	}

	Value(OrderedMap<std::string, Value *> input) :
		dataType(ValueType::DICTIONARY), dictionary(input)
	{
	}
};



typedef char TByte;

typedef std::vector<TByte> TToken;

class Bencoding {

public:  
	/** Tree used for storing the torrent file */
	Value tree;

	/** Constructor */
	Bencoding();

	TToken Encode(const std::string key);
	Value Decode(const TByte *encoded, int length);

private:

	/** Checks if string is a number
	*
	* @param s a ref to a string .
	* @return true if the string is number.
	*/
	bool is_number(const std::string& s);

	/** Fill the tree according to the tyoes in tokenize list.
	*
	* @param tokens a list contains all the variables that hadn't check yet.
	* @return Value a vlue containg the specific value as the format.
	*/
	Value FillTree(std::vector<TToken>& tokens);
	
	/** Decode an integer
	*
	* @param tokens a list contains all the variables that hadn't check yet.
	* @return Value a value containg integer.
	*/
	Value decodeInt(std::vector<TToken>& tokens);
	
	/** Decode an integer
	*
	* @param tokens a list contains all the variables that hadn't check yet.
	* @return Value a value containg string.
	*/
	Value decodeString(std::vector<TToken>& tokens);

	/** Decode an integer
	*
	* @param tokens a list contains all the variables that hadn't check yet.
	* @return Value a value containg list.
	*/
	Value decodeList(std::vector<TToken>& tokens);

	/** Decode an integer
	*
	* @param tokens a list contains all the variables that hadn't check yet.
	* @return Value a value containg dictionary.
	*/
	Value decodeDict(std::vector<TToken>& tokens);
	
	/** Transfer torrent file from txt into a list of tokens
	*
	* @param encoded a pointer to the content of torrent file.
	* @param tokens a ref for moving all the tokens to.
	* @param length the length of the file in bytes.
	* @param counter a counter.
	*/
	void Tokenize(TByte *encoded, std::vector<TToken>& tokens, int length, int counter);

	TToken FillContent(Value map);

	TToken EncodeString(Value map);

	TToken EncodeList(Value map);

	TToken EncodeDictionary(Value map);

	TToken EncodeInteger(Value map);

	TToken EncodeKey(std::string text);
	
};