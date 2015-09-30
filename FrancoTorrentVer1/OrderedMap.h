#pragma once

#include <string>

struct Value;


class OrderedMap
{
	
public:

	OrderedMap(void) 
	{}

	void Insert(std::string key, Value * value) 
	{
		auto val = make_pair(key, value);
		// insert in map
		auto pr = dictionary.insert(val);

		Value * valuePtr = dictionary[key];


		auto find_it = dictionary.find(key);
		
	//	Pointers pointers(find_it->first, valuePtr);

		dictPtrs.push_back(val);
			// = ;
	};

	Value * getValueByKey(std::string key)
	{
		Value * v = dictionary[key];

		return v;
	};

	Value * getValueByIndex(int index)
	{
		return  dictPtrs[index].second;
	};

	std::string  getKeyByIndex(int index)
	{
		return dictPtrs[index].first;
	};

	

private:


	typedef std::map<std::string, Value *> DictionaryMap;
	DictionaryMap dictionary;

	typedef std::pair<std::string, Value*> PairType;
	std::vector<PairType> dictPtrs;

};
