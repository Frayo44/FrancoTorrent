#pragma once

#include <string>

struct Value;

struct Pointers
{

public:
	std::string * text;
	Value * value;

	Pointers(std::string * text, Value * value)
		: text(text), value(value)
	{
		
	}

};

class OrderedMap
{
	
public:

	OrderedMap(void){}

	void Insert(std::string key, Value * value) 
	{
		dictionary[key] = value;

		Value * valuePtr = dictionary[key];


		auto find_it = dictionary.find(key);
		
			Pointers pointers((std::string *)&find_it->first, valuePtr);
			dictPtrs.push_back(pointers);
			// = ;
		



		
		
	};

	Value * getValueByKey(std::string key)
	{
		Value * v = dictionary[key];

		return v;
	};

	Value * getValueByIndex(int index)
	{
		return  dictPtrs[index].value;
	};

	std::string * getKeyByIndex(int index)
	{
		return dictPtrs[index].text;
	};

	

private:
	std::string keyy;
	std::map<std::string, Value *> dictionary;
	std::vector<Pointers> dictPtrs;
};
