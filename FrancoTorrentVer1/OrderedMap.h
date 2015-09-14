#pragma once

#include <map>
#include <string>
#include <vector>
#include "Bencoding.h"

struct Pointers
{
//	std::pair <std::string, Value> * product1;
	std::string * text;
	Value * value;

public:
	Pointers(std::string * text, Value * value)
	{
		this->text = text;
		this->value = value;
	}


};

class OrderedMap
{
public:

	void Insert(std::string key, Value value) 
	{
		dictionary[key] = value;
		
		Value * valuePtr = & dictionary[key];
		Pointers pointers((std::string *) valuePtr - 1, valuePtr);
		dictPtrs.push_back(pointers);
	};

private:
	std::map<std::string, Value> dictionary;
	std::vector<Pointers> dictPtrs;
};