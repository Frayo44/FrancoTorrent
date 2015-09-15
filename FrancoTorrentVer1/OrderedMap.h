#pragma once

#include <string>
#include "Bencoding.h"

struct Pointers
{

	
public:
	std::string * text;
	Value * value;

	Pointers(std::string * text, Value * value)
		: text(text), value(value)
	{
		//this->text = text;
		//this->value = value;
	}


};

class OrderedMap
{
	


public:

	OrderedMap(void){}

	void Insert(std::string key, Value value) 
	{
		dictionary[key] = value;
		
		Value * valuePtr = & dictionary[key];
		Pointers pointers((std::string *) valuePtr - 1, valuePtr);
		dictPtrs.push_back(pointers);
	};

	Value getValueByKey(std::string key)
	{
		Value v = dictionary[key];

		return v;
	};

	Value getValueByIndex(int index)
	{
		return (Value) * (dictPtrs[index].value);
	};

	std::string getKeyByIndex(int index)
	{
		return (std::string) * dictPtrs[index].text;
	}

	

private:
	std::map<std::string, Value> dictionary;
	std::vector<Pointers> dictPtrs;
};
